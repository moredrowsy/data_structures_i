#include "../include/sql_table.h"

namespace sql {

SQLTable::SQLTable(std::string table_name)
    : _rec_count(0), _table_name(table_name + ".tbl"), _record(_table_name) {
    init_table();

    std::string old_name = _table_name;

    if(!_rec_count) {  // try to open with all caps
        std::string temp = _table_name.substr(0, _table_name.size() - 4);
        std::transform(temp.begin(), temp.end(), _table_name.begin(),
                       ::toupper);
        _record.set_fname(_table_name);
        init_table();
    }

    if(!_rec_count) {  // create new table file if second try fails
        _table_name = old_name;
        _record.set_fname(old_name);
        std::ofstream file(old_name.c_str(),
                           std::ios::binary | std::ios::trunc);
        file.close();
    }
}

SQLTable::SQLTable(std::string table_name,
                   const std::vector<std::string>& fields)
    : _rec_count(0), _table_name(table_name + ".tbl"), _record(_table_name) {
    std::ofstream file(_table_name.c_str(), std::ios::binary | std::ios::trunc);
    file.close();
    _record.write(fields, _rec_count);
    init_table();
}

std::size_t SQLTable::field_count() const { return _pos_to_fields.size(); }

std::size_t SQLTable::size() const { return _rec_count; }

void SQLTable::delete_table() {
    std::remove(_table_name.c_str());
    _rec_count = 0;
    _map.clear();
    _pos_to_fields.clear();
    _field_to_pos.clear();
    _table_name.clear();
    _record.set_fname("");
}

bool SQLTable::contains(const std::string& field_name) const {
    return _field_to_pos.contains(field_name);
}

const FieldMap& SQLTable::map() const { return _map; }

bool SQLTable::insert(const std::vector<std::string>& values) {
    std::string field_name;

    long pos = _record.write(values, _rec_count++);

    for(std::size_t i = 0; i < values.size(); ++i) {
        if(values[i].empty())  // if the next value is empty string, stop
            break;
        else {  // i + 1! position 0 reserved for if map record is deleted or
                // not
            field_name = _pos_to_fields[i];
            _map[field_name][values[i]] += pos;
        }
    }

    return true;
}

bool SQLTable::is_match_fields(const std::vector<std::string>& list) {
    for(const auto& a : list)
        if(!contains(a)) return false;

    return true;
}

bool SQLTable::select(const std::vector<std::string>& fields_list,
                      QueueTokens& infix, SQLTable& new_table) {
    QueueTokens postfix;              // postfix from WHERE condition
    set_ptr result;                   // final set result
    std::vector<std::string> fields;  // fields to copy to new_table
    std::vector<std::string> values;  // values read from record
    fields.reserve(REC_ROW);
    values.reserve(REC_ROW);

    // get all fields (in order) from current table
    for(auto const& a : _pos_to_fields) fields.push_back(a.value);

    // copy all field labels to new table
    new_table.update_fields(fields);

    if(!infix.empty()) {                   // if infix exist
        infix_to_postfix(infix, postfix);  // convert infix to postfix
        eval_postfix(postfix, result);     // eval postfix for final result set

        // add records only in result set to new table
        for(const auto& i : *result) {
            if(i > 0 && _record.read(values, i)) {  // skip i == 0 b/c
                new_table.insert(values);           // b/c 0 represent empty set
                values.clear();
            }
        }
    } else {                                    // if no infix
        for(long i = 1; i < _rec_count; ++i) {  // add all records to new table
            if(_record.read(values, i)) {
                new_table.insert(values);
                values.clear();
            }
        }
    }

    // print all records in new table but display only given field list
    new_table.print(fields_list);

    return true;
}

void SQLTable::make_equal_set(const std::string& field,
                              const std::string& value, set_ptr& result) {
    if(_map[field].contains(value))
        for(const auto& a : _map[field][value]) result->insert(a);
    else
        result->insert(0);
}

void SQLTable::make_less_set(const std::string& field, const std::string& value,
                             set_ptr& result) {
    auto found = _map[field].find(value);

    if(found)
        for(auto it = _map[field].begin(); it != _map[field].end(); ++it) {
            if(it->key == found->key) break;  // break when key is the same
            result->insert(*it->value);
        }
    else
        result->insert(0);
}

void SQLTable::make_less_eq_set(const std::string& field,
                                const std::string& value, set_ptr& result) {
    auto found = _map[field].find(value);
    found.next_key();  // increment to next key

    if(found)
        for(auto it = _map[field].begin(); it != _map[field].end(); ++it) {
            if(it->key == found->key) break;  // break when 'next key' is same
            result->insert(*it->value);
        }
    else
        result->insert(0);
}

void SQLTable::make_greater_set(const std::string& field,
                                const std::string& value, set_ptr& result) {
    auto found = _map[field].find(value);
    found.next_key();  // increment to next key

    if(found)
        while(found) {
            result->insert(*found->value);
            ++found;
        }
    else
        result->insert(0);
}

void SQLTable::make_greater_eq_set(const std::string& field,
                                   const std::string& value, set_ptr& result) {
    auto found = _map[field].find(value);

    if(found)
        while(found) {
            result->insert(*found->value);
            ++found;
        }
    else
        result->insert(0);
}

void SQLTable::print(const std::vector<std::string>& field_names, int width) {
    const std::vector<std::string>* field_list = &field_names;
    std::vector<std::string>* v = nullptr;

    // if asterisk, create field names with all values
    if(field_names[0] == "*") {
        v = new std::vector<std::string>;
        for(const auto& a : _pos_to_fields) v->push_back(a.value);
        field_list = v;
    }

    print_header(*field_list, width);  // print header

    // print all the records in the table
    for(int i = 1; i < _rec_count; ++i) print_rec(i, *field_list);

    delete v;  // delete temp v
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

    if(_record.read(values, rec_pos)) {
        for(std::size_t i = 0; i < size; ++i) {
            field_name = field_names[i];
            field_pos = _field_to_pos[field_name];
            value = values[field_pos];
            std::cout << std::setw(width) << truncate(value, width) << ' ';
        }
        std::cout << std::endl;
    } else {
        std::cout << "ERROR: Record does not exist at rec #" << rec_pos
                  << std::endl;
    }
}

void SQLTable::print_header(const std::vector<std::string>& field_names,
                            int width) {
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

    if(_record.read(field_names, 0)) {  // read into vector for rec 0
        if(!_rec_count) ++_rec_count;
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

void SQLTable::infix_to_postfix(QueueTokens& infix, QueueTokens& postfix) {
    StackTokens operators;  // temp stack to hold operators

    while(!infix.empty()) {
        auto t = infix.pop();  // get shared ptr to SQLToken

        if(t->type() < 0)  //  add to postfix if SQLToken is operand
            postfix.push(t);
        else {                              // when SQLToken is operator
            if(operators.empty())           // and is empty
                operators.push(t);          // push to operator stack
            else {                          // if it's not empty
                auto op = operators.top();  // get op in stack

                if(op->type() > t->type()) {  // if op is > current op
                    // keep popping operator stack until op in stack is less
                    // than current op
                    while(!operators.empty() && op->type() >= t->type()) {
                        postfix.push(operators.pop());

                        if(!operators.empty()) op = operators.top();
                    }
                    operators.push(t);  // add current op to operator stack
                } else
                    operators.push(t);  // add if op in stack is less than curr
            }
        }
    }

    while(!operators.empty()) postfix.push(operators.pop());  // pop remaining
}

void SQLTable::eval_postfix(QueueTokens& postfix, set_ptr& result_set) {
    StackTokens operands;  // temp stack to hold operands

    while(!postfix.empty()) {
        auto t = postfix.pop();

        if(t->type() < 0)  // if SQLToken is operand, push to operand stack
            operands.push(t);
        else {  // when operators, then there must be at least two operands
            auto second = operands.pop();  // pop the top two operands
            auto first = operands.pop();
            set_ptr result = std::make_shared<set::Set<long>>();
            token_ptr token_set = std::make_shared<SQLToken>("new_set");

            // perform set creation or set modification
            switch(t->subtype()) {
                case TOKEN_OP_OR:
                    *first->data() += *second->data();
                    operands.push(first);
                    break;
                case TOKEN_OP_AND:
                    first->data()->intersect(*second->data(), *result);
                    token_set->data() = result;
                    operands.push(token_set);
                    break;
                case TOKEN_R_EQ:
                    make_equal_set(first->string(), second->string(), result);
                    token_set->data() = result;
                    operands.push(token_set);
                    break;
                case TOKEN_R_L:
                    make_less_set(first->string(), second->string(), result);
                    token_set->data() = result;
                    operands.push(token_set);
                    break;
                case TOKEN_R_LEQ:
                    make_less_eq_set(first->string(), second->string(), result);
                    token_set->data() = result;
                    operands.push(token_set);
                    break;
                case TOKEN_R_G:
                    make_greater_set(first->string(), second->string(), result);
                    token_set->data() = result;
                    operands.push(token_set);
                    break;
                case TOKEN_R_GEQ:
                    make_greater_eq_set(first->string(), second->string(),
                                        result);
                    token_set->data() = result;
                    operands.push(token_set);
                    break;
                default:
                    break;
            }
        }
    }

    result_set = operands.pop()->data();  // one operand left, which is result
}

void SQLTable::update_fields(const std::vector<std::string>& fields) {
    _record.write(fields, 0);

    _pos_to_fields.clear();
    _field_to_pos.clear();
    init_fields();
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
