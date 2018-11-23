#include "../include/sql_table.h"

namespace sql {

SQLTable::SQLTable(std::string table_name)
    : _rec_count(0), _table_name(table_name + ".tbl"), _record(_table_name) {
    init_table();

    if(!_rec_count) {  // try to open with all caps
        std::string temp = _table_name.substr(0, _table_name.size() - 4);
        std::transform(temp.begin(), temp.end(), _table_name.begin(),
                       ::toupper);
        _record.set_fname(_table_name);
        init_table();
    }
}

SQLTable::SQLTable(std::string table_name,
                   const std::vector<std::string>& values)
    : _rec_count(0), _table_name(table_name + ".tbl"), _record(_table_name) {
    std::ofstream file(_table_name.c_str(), std::ios::binary | std::ios::trunc);
    file.close();
    _record.write(values, _rec_count);
    init_table();
}

std::size_t SQLTable::field_count() const { return _pos_to_fields.size(); }

std::size_t SQLTable::size() const { return _rec_count; }

bool SQLTable::contains(const std::string& field_name) const {
    return _field_to_pos.contains(field_name);
}

const SQLTable::FieldMap& SQLTable::map() const { return _map; }

bool SQLTable::insert(const std::vector<std::string>& values) {
    std::string field_name;

    long pos = _record.write(values, _rec_count++);

    for(std::size_t i = 0; i < values.size(); ++i) {
        // i + 1! position 0 reserved for if map record is deleted or not
        field_name = _pos_to_fields[i];
        _map[field_name][values[i]] += pos;
    }

    return true;
}

void SQLTable::make_equal_set(const std::string& field,
                              const std::string& value,
                              set::Set<long>& result) {
    for(const auto& a : _map[field][value]) result[a];
}

void SQLTable::make_less_set(const std::string& field, const std::string& value,
                             set::Set<long>& result) {
    auto found = _map[field].find(value);

    for(auto it = _map[field].begin(); it != _map[field].end(); ++it) {
        if(it->key == found->key) break;  // break when key is the same
        result[*it->value];
    }
}

void SQLTable::make_less_equal_set(const std::string& field,
                                   const std::string& value,
                                   set::Set<long>& result) {
    auto found = _map[field].find(value);
    found.next_key();  // increment to next key

    for(auto it = _map[field].begin(); it != _map[field].end(); ++it) {
        if(it->key == found->key) break;  // break when 'next key' is same
        result[*it->value];
    }
}

void SQLTable::make_greater_set(const std::string& field,
                                const std::string& value,
                                set::Set<long>& result) {
    auto found = _map[field].find(value);
    found.next_key();  // increment to next key

    while(found) {
        result[*found->value];
        ++found;
    }
}

void SQLTable::make_greater_equal_set(const std::string& field,
                                      const std::string& value,
                                      set::Set<long>& result) {
    auto found = _map[field].find(value);

    while(found) {
        std::cout << found->key << std::endl;
        result[*found->value];
        ++found;
    }
}

void SQLTable::print_all(int width) {
    long rec_pos = 1;
    std::size_t size = _pos_to_fields.size();
    std::vector<std::string> values;
    values.reserve(REC_ROW);

    std::cout.setf(std::ios::left);

    // print header
    for(std::size_t i = 0; i < size; ++i)
        std::cout << std::setw(width) << truncate(_pos_to_fields[i], width)
                  << ' ';
    std::cout << std::endl;

    // print bar
    for(std::size_t i = 0; i < size; ++i)
        std::cout << std::string(width, '-') << ' ';
    std::cout << std::endl;

    // print data: read record into vector and print vector
    while(_record.read(values, rec_pos++)) {
        for(std::size_t i = 0; i < size; ++i)
            std::cout << std::setw(width) << truncate(values[i], width) << ' ';
        std::cout << std::endl;

        values.clear();
    }
}

void SQLTable::print_rec(long rec_pos,
                         const std::vector<std::string>& field_names,
                         int width) {
    int field_pos;                          // field pos
    std::size_t size = field_names.size();  // user given field list's size
    std::string field_name;                 // field name from user
    std::vector<std::string> values;        // values read from record
    std::string value;                      // data
    values.reserve(REC_ROW);

    std::cout.setf(std::ios::left);

    _record.read(values, rec_pos);

    for(std::size_t i = 0; i < size; ++i) {
        field_name = field_names[i];
        field_pos = _field_to_pos[field_name];
        value = values[field_pos];
        std::cout << std::setw(width) << truncate(value, width) << ' ';
    }
    std::cout << std::endl;
}

void SQLTable::print_specific_header(
    const std::vector<std::string>& field_names, int width) {
    print_rec(0, field_names);

    std::cout.setf(std::ios::left);

    std::size_t size = field_names.size();  // user given field list's size
    for(std::size_t i = 0; i < size; ++i)
        std::cout << std::string(width, '-') << ' ';
    std::cout << std::endl;
}

void SQLTable::init_table() {
    init_fields();
    init_data();
}

void SQLTable::init_fields() {
    std::vector<std::string> field_names;
    field_names.reserve(REC_ROW);

    if(_record.read(field_names, _rec_count)) {  // read into vector for rec 0
        ++_rec_count;
        std::size_t size = field_names.size();

        // populate field names to FieldPosMap
        for(std::size_t i = 0; i < size; ++i) {
            if(field_names[i].empty()) break;    // stop when empty
            _pos_to_fields[i] = field_names[i];  // map pos to field name
            _field_to_pos[field_names[i]] += i;  // map field name to pos
        }
    }
}

void SQLTable::init_data() {
    std::size_t size = _pos_to_fields.size();
    std::vector<std::string> values;
    values.reserve(REC_ROW);
    assert(values.size() <= size);

    // keep reading until record returns 0
    while(_record.read(values, _rec_count)) {  // read record to vector
        // populate table; ie: _map["lName"]["Gates"] += 1;
        for(std::size_t i = 0; i < size; ++i)
            _map[_pos_to_fields[i]][std::move(values.at(i))] += _rec_count;

        values.clear();
        ++_rec_count;  // update record counts
    }
}

std::string SQLTable::truncate(std::string str, size_t width,
                               bool show_ellipsis) {
    if(str.size() > width) {
        if(show_ellipsis)
            return str.substr(0, width - 3) + "...";
        else
            return str.substr(0, width);
    }
    return str;
}

}  // namespace sql
