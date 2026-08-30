# Financial ETL Pipeline & Data Analytics

A production-grade Extract, Transform, Load (ETL) pipeline built with Python and SQLite. This repository demonstrates core data engineering principles, ensuring data integrity, type safety, and efficient relational database management for financial datasets.

**Core Architecture & Engineering Principles**
* **Object-Oriented Design (OOP):** The entire ETL process is encapsulated within a scalable `FinancialETL` class, allowing for easy integration of future data sources or alternative databases (e.g., PostgreSQL).
* **Data Validation:** Utilizes `Pydantic` to enforce strict type checking and validation during the Transform phase, preventing corrupted or incomplete data from reaching the database.
* **Safe Database Transactions:** Implements Python's context managers (`with` statements) for SQLite operations to ensure safe connection handling and prevent memory leaks.
* **Production Logging:** Replaces standard print statements with the `logging` module to track extraction metrics, validation failures, and database transactions in real-time.
* **SQL Analytics:** Demonstrates relational database querying utilizing aggregation functions (`AVG`, `COUNT`) and grouping (`GROUP BY`) to extract meaningful business insights from raw data.

**Tech Stack**
* **Language:** Python 3
* **Validation:** Pydantic
* **Database:** SQLite3 (Built-in)
* **Monitoring:** Python Logging

**How to Run**
1. Clone the repository and install dependencies:
git clone https://github.com/yourusername/financial-etl-pipeline.git
cd financial-etl-pipeline
pip install -r requirements.txt

2. Execute the pipeline:
python src/etl_pipeline.py

The script will automatically generate a local `financial_data.db` SQLite database, perform the ETL process, and output an aggregated SQL analytics summary to the terminal.