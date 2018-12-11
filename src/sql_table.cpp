#include "../include/sql_table.h"

namespace sql {

/*******************************************************************************
 * DESCRIPTION:
 *  Constructor which attempts to open a file with the supplied table name.
 *  If file does not exist, create a new table file.
 *
 * PRE-CONDITIONS:
 *  const std::string& table_name: table name
 *
 * POST-CONDITIONS:
 *  initializations
 *
 * RETURN:
 *  none
 ******************************************************************************/
SQLTable::SQLTable(const std::string& table_name)
    : _rec_count(0),
      _table_name(table_name),
      _ext(".tbl"),
      _fname(_table_name + _ext),
      _record(_fname) {
    init_table();

    std::string old_name = _table_name;

    if(!_rec_count) {  // try to open with all caps
        std::transform(_table_name.begin(), _table_name.end(),
                       _table_name.begin(), ::toupper);
        _fname = _table_name + _ext;
        _record.set_fname(_fname);
        init_table();
    }

    if(!_rec_count) {  // create new table file if second try fails
        _table_name = old_name;
        _fname = _table_name + _ext;
        _record.set_fname(_fname);
        std::ofstream file(_fname.c_str(), std::ios::binary | std::ios::trunc);
        file.close();
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Constructor create a new file with the given field labels.
 *
 * PRE-CONDITIONS:
 *  const std::string& table_name         : table name
 *  const std::vector<std::string>& fields: list of fields
 *
 * POST-CONDITIONS:
 *  initializations
 *
 * RETURN:
 *  none
 ******************************************************************************/
SQLTable::SQLTable(const std::string& table_name,
                   const std::vector<std::string>& fields)
    : _rec_count(0),
      _table_name(table_name),
      _ext(".tbl"),
      _fname(_table_name + _ext),
      _record(_fname) {
    std::ofstream file(_fname.c_str(), std::ios::binary | std::ios::trunc);
    file.close();
    _record.write(fields, _rec_count);
    init_table();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the number of fields in table.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
std::size_t SQLTable::field_count() const { return _pos_to_fields.size(); }

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the number of records in table.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
std::size_t SQLTable::size() const { return _rec_count; }

/*******************************************************************************
 * DESCRIPTION:
 *  Explicit table deletion, which removes the associated table file and clears
 *  all Maps.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  Table file deleted and maps/other variables are cleared
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLTable::delete_table() {
    std::remove(_fname.c_str());
    _rec_count = 0;
    _map.clear();
    _pos_to_fields.clear();
    _field_to_pos.clear();
    _table_name.clear();
    _fname.clear();
    _record.set_fname("");
}

/*******************************************************************************
 * DESCRIPTION:
 *  RChecks whether field name exists in table.
 *
 * PRE-CONDITIONS:
 *  const std::string& field_name: field name
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
bool SQLTable::contains(const std::string& field_name) const {
    return _field_to_pos.contains(field_name);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the map which holds all IndexMaps.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
const FieldMap& SQLTable::map() const { return _map; }

/*******************************************************************************
 * DESCRIPTION:
 *  Performs insertion of a vector of values into table.
 *
 * PRE-CONDITIONS:
 *  const std::vector<std::string>& values: list of values
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
bool SQLTable::insert(const std::vector<std::string>& values) {
    std::string field_name;

    long pos = _record.write(values, _rec_count++);

    for(std::size_t i = 0; i < values.size(); ++i) {
        if(values[i].empty())  // if the next value is empty string, stop
            break;
        else {
            field_name = _pos_to_fields[i];
            _map[field_name][values[i]] += pos;
        }
    }

    return true;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if all fields in list is contained in table.
 *
 * PRE-CONDITIONS:
 *  const std::vector<std::string>& fields: list of field labels
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
bool SQLTable::is_match_fields(const std::vector<std::string>& fields) {
    for(const auto& a : fields)
        if(!contains(a)) return false;

    return true;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns a new table with selected data.
 *
 * PRE-CONDITIONS:
 *  const std::vector<std::string>& fields_list: fields list
 *  QueueTokens& infix                         : queue of SQLTokens
 *  SQLTable& new_table                        : new table
 *
 * POST-CONDITIONS:
 *  SQLTable& new_table: old table deleted and populated with new data
 *
 * RETURN:
 *  none
 ******************************************************************************/
bool SQLTable::select(const std::vector<std::string>& fields_list,
                      QueueTokens& infix, SQLTable& new_table) {
    QueueTokens postfix;              // postfix for WHERE condition
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

/*******************************************************************************
 * DESCRIPTION:
 *  Return by ref a set of records of specified conditions.
 *
 * PRE-CONDITIONS:
 *  const std::string& field: target field
 *  const std::string& value: value of field
 *  set_ptr& result         : empty set of records
 *
 * POST-CONDITIONS:
 *  set_ptr& result         : populated record positions
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLTable::make_equal_set(const std::string& field,
                              const std::string& value, set_ptr& result) {
    if(_map[field].contains(value)) *result += _map[field][value];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return by ref a set of records of specified conditions.
 *
 * PRE-CONDITIONS:
 *  const std::string& field: target field
 *  const std::string& value: value of field
 *  set_ptr& result         : empty set of records
 *
 * POST-CONDITIONS:
 *  set_ptr& result         : populated record positions
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLTable::make_less_set(const std::string& field, const std::string& value,
                             set_ptr& result) {
    auto low = _map[field].lower_bound(value);
    auto beg = _map[field].begin();

    while(beg != low) {
        *result += beg->value;
        ++beg;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return by ref a set of records of specified conditions.
 *
 * PRE-CONDITIONS:
 *  const std::string& field: target field
 *  const std::string& value: value of field
 *  set_ptr& result         : empty set of records
 *
 * POST-CONDITIONS:
 *  set_ptr& result         : populated record positions
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLTable::make_less_eq_set(const std::string& field,
                                const std::string& value, set_ptr& result) {
    auto upp = _map[field].upper_bound(value);
    auto beg = _map[field].begin();

    while(beg != upp) {
        *result += beg->value;
        ++beg;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return by ref a set of records of specified conditions.
 *
 * PRE-CONDITIONS:
 *  const std::string& field: target field
 *  const std::string& value: value of field
 *  set_ptr& result         : empty set of records
 *
 * POST-CONDITIONS:
 *  set_ptr& result         : populated record positions
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLTable::make_greater_set(const std::string& field,
                                const std::string& value, set_ptr& result) {
    auto upp = _map[field].upper_bound(value);
    auto end = _map[field].end();

    while(upp != end) {
        *result += upp->value;
        ++upp;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return by ref a set of records of specified conditions.
 *
 * PRE-CONDITIONS:
 *  const std::string& field: target field
 *  const std::string& value: value of field
 *  set_ptr& result         : empty set of records
 *
 * POST-CONDITIONS:
 *  set_ptr& result         : populated record positions
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLTable::make_greater_eq_set(const std::string& field,
                                   const std::string& value, set_ptr& result) {
    auto low = _map[field].lower_bound(value);
    auto end = _map[field].end();

    while(low != end) {
        *result += low->value;
        ++low;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Print all records
 *
 * PRE-CONDITIONS:
 *  const std::vector<std::string>& field_names: fields to display
 *  int width                                  : maximum width to display
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Print record of specified position.
 *
 * PRE-CONDITIONS:
 *  long rec_pos                               : record position
 *  const std::vector<std::string>& field_names: fields to display
 *  int width                                  : maximum width to display
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Print header.
 *
 * PRE-CONDITIONS:
 *  const std::vector<std::string>& field_names: fields to display
 *  int width                                  : maximum width to display
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLTable::print_header(const std::vector<std::string>& field_names,
                            int width) {
    print_rec(0, field_names);

    std::cout.setf(std::ios::left);

    std::size_t size = field_names.size();  // user given field list's size
    for(std::size_t i = 0; i < size; ++i)
        std::cout << std::string(width, '-') << ' ';
    std::cout << std::endl;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Initialize all table fields and data.
 *
 * PRE-CONDITIONS:
 *  std::string _fname: valid table file name
 *
 * POST-CONDITIONS:
 *  Maps initialized with data
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLTable::init_table() {
    init_fields();
    init_data();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Initialize position to field and field to position maps. Stores field label
 *  order.
 *
 * PRE-CONDITIONS:
 *  std::string _fname: valid table file name
 *
 * POST-CONDITIONS:
 *  Maps initialized with data
 *
 * RETURN:
 *  none
 ******************************************************************************/
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
            _field_to_pos[field_names[i]] = i;   // map field name to pos
        }
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Initialize IndexMaps to FieldMap from table file.
 *
 * PRE-CONDITIONS:
 *  std::string _fname: valid table file name
 *
 * POST-CONDITIONS:
 *  Maps initialized with data
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Convert infix expression to postfix expression.
 *
 * PRE-CONDITIONS:
 *  QueueTokens& infix  : Queue of SQL Tokens from parse tree with infix order
 *  QueueTokens& postfix: empty
 *
 * POST-CONDITIONS:
 *  QueueTokens& infix  : empty
 *  QueueTokens& postfix: Queue of SQL Tokens with postfix order
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Evaluate postfix expression and convert it to record positions to set.
 *
 * PRE-CONDITIONS:
 *  QueueTokens& postfix: Queue of SQL Tokens with postfix order
 *  set_ptr& result_set : empty set
 *
 * POST-CONDITIONS:
 *  QueueTokens& postfix: empty
 *  set_ptr& result_set : set with record positions (may be emtpy set)
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Update field labels and position information.
 *
 * PRE-CONDITIONS:
 *  const std::vector<std::string>& fields: list of fields
 *
 * POST-CONDITIONS:
 *  field maps are cleared and updated to with new field list
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLTable::update_fields(const std::vector<std::string>& fields) {
    _record.write(fields, 0);

    _pos_to_fields.clear();
    _field_to_pos.clear();
    init_fields();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Update field labels and position information.
 *
 * PRE-CONDITIONS:
 *  std::string str   : target string
 *  size_t width      : width of string
 *  bool show_ellipsis: flag to show ellipsis when truncation occurs
 *
 * POST-CONDITIONS:
 *  std::string str: truncated to width if overlimit
 *
 * RETURN:
 *  none
 ******************************************************************************/
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
