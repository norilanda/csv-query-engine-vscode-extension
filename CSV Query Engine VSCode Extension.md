# CSV Query Engine VSCode Extension Specification

## Short Description
**Project name**: CSV Query Engine VSCode Extension  
**Description**: A VS Code extension for querying large CSV files using a custom SQL-like language with stream-based processing ~~and indexing~~. The frontend will be written in TypeScript and the backend in C++.  
**Target files**: CSV files from 1MB to 4GB+; smaller files are also supported.

## Goals/Non-Goals

### Goals:
- Query multi-GB CSV files without loading the entire file into memory
- Output query results to a new CSV file
- Support essential SQL operations (SELECT, WHERE, LIMIT). Detailed specifications are in the "Query Language Specification" section
- ~~Build indices for performance improvement. For this project, only BITMAP indices for columns with low cardinality will be implemented. Details are in the "Indices Specification" section~~

### Non-Goals:
- Full SQL compliance (no JOINs, GROUP BY, subqueries, or transactions)
- Editing, updating, or viewing* input CSV files (input CSV files are read-only; *VSCode does not open large files by default, and this project will not address this limitation)

## Query Language Specification

### Grammar
```
query          ::= select_clause [where_clause] +✅[order_by_clause]✅+ [limit_clause]

select_clause  ::= "SELECT" column_list | "*"
column_list    ::= column_name ("," column_name)*
column_name    ::= IDENTIFIER

where_clause   ::= "WHERE" condition
condition      ::= comparison (("AND" | "OR") comparison)*
comparison     ::= column_name operator value
operator       ::= "=" | "!=" | "<" | ">" | "<=" | ">="
value          ::= STRING_LITERAL | NUMBER | BOOLEAN

+✅
order_by_clause ::= "ORDER" "BY" order_item ("," order_item)*
order_item      ::= column_name [sort_direction]
sort_direction  ::= "ASC" | "DESC"
✅+

limit_clause   ::= "LIMIT" NUMBER
```

### Supported Keywords
```
SELECT, WHERE, LIMIT, AND, OR, ORDER BY, ASC, DESC
```

### Example Queries
```sql
-- Basic SELECT
SELECT name, age, salary

-- Filtering
SELECT * WHERE age > 25 AND country = 'USA'

-- Limiting
SELECT name, salary
LIMIT 10
```
~~## Indices Specification~~
- ~~**Index creation**: Users will create indices on specific columns manually (via command palette)~~
- ~~**Storage**: Each index will be stored in a separate file within a `.csvqidx` folder~~
- ~~**Usage**: Created indices will be used automatically to optimize query execution~~
- ~~**Supported index types**:~~
    - ~~**Bitmap Index**: For columns with low cardinality~~
    - ~~**No Index**: Default option for columns without indices~~

## Frontend Specification
- A command that will trigger the opening of a dialog window where the user can provide the query. The command will only work if the currently open document in VSCode is a .csv file;
- A command that will allow a user to view the column names. VSCode does not open large files by default, and a user needs a way to check the column names somehow, so this command will address this issue;
- ~~A command that will trigger the opening of a dialog window where the user can specify for which column he wants to build a bitmap index;~~
- Warnings, Errors, and Progress bar will be displayed in a user-friendly way using VSCode API.