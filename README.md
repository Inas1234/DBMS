# DBMS

This project is a Database Management System (DBMS) implemented in C++. It provides functionalities for managing and querying databases, including a custom query language interpreter.

## Features

- **Custom Query Language Interpreter**: Supports executing queries written in a custom query language (`mlinql`).
- **User Management**: Handles user data stored in JSON format.
- **Modular Design**: Organized with separate directories for source code, libraries, and data.

## Project Structure

- `src/`: Contains the source code files.
- `lib/include/nlohmann/`: Includes the [nlohmann JSON library](https://github.com/nlohmann/json) for JSON parsing.
- `data/`: Stores data files used by the DBMS.
- `main.cpp`: The main entry point of the application.
- `Makefile`: Build script for compiling the project.
- `users.json`: JSON file containing user data.
- `test.mql`: Sample query file for testing the query language interpreter.
- `DBMS docs.pdf`: Documentation file providing detailed information about the project.

## Getting Started

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/Inas1234/DBMS.git
   ```

2. **Navigate to the Project Directory**:

   ```bash
   cd DBMS
   ```

3. **Build the Project**:

   Ensure you have a C++ compiler installed. Then, run:

   ```bash
   make
   ```

4. **Run the Application**:

   After building, execute the main program:

   ```bash
   ./main
   ```

## Usage

- **Executing Queries**:

   You can write queries in the custom query language (`mlinql`) and execute them using the interpreter. Refer to `test.mql` for example queries.

- **User Management**:

   User data is managed through the `users.json` file. Ensure this file is properly formatted to avoid errors.

## Dependencies

- **nlohmann JSON Library**:

   The project utilizes the [nlohmann JSON library](https://github.com/nlohmann/json) for JSON parsing. The necessary headers are included in the `lib/include/nlohmann/` directory.

## Documentation

For detailed information about the project's design, features, and usage, refer to the `DBMS docs.pdf` file included in the repository.

. 
