import sqlite3
import logging
from typing import List, Optional
from pydantic import BaseModel, ValidationError

# Configure standard logging for production-level monitoring
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

# Define data schema for incoming raw data to ensure type safety
class CurrencyData(BaseModel):
    currency_pair: str
    rate: Optional[float]
    status: str

class FinancialETL:
    """
    Encapsulates the Extract, Transform, Load (ETL) logic within an OOP structure.
    """
    def __init__(self, db_name: str = "financial_data.db"):
        self.db_name = db_name
        self._setup_database()

    def _setup_database(self):
        """Safely initializes the SQLite database and creates the schema."""
        # Using context manager (with) to automatically handle connection closing
        with sqlite3.connect(self.db_name) as conn:
            cursor = conn.cursor()
            cursor.execute("""
                CREATE TABLE IF NOT EXISTS exchange_rates (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    pair TEXT NOT NULL,
                    rate REAL NOT NULL,
                    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
                )
            """)
            conn.commit()
        logging.info("Database schema initialized successfully.")

    def extract(self) -> List[dict]:
        """Simulates data extraction from an external financial API."""
        logging.info("Extracting data from mock API...")
        return [
            {"currency_pair": "USD/TRY", "rate": 34.20, "status": "active"},
            {"currency_pair": "EUR/TRY", "rate": None, "status": "error"}, # Corrupted Data
            {"currency_pair": "GBP/TRY", "rate": 45.10, "status": "active"},
            {"currency_pair": "CHF/TRY", "rate": 46.50, "status": "active"}
        ]

    def transform(self, raw_data: List[dict]) -> List[tuple]:
        """Validates and cleans the extracted data using Pydantic."""
        logging.info("Transforming and validating data...")
        valid_data = []
        
        for item in raw_data:
            try:
                # Parse and validate the dictionary using the Pydantic model
                parsed_item = CurrencyData(**item)
                
                # Business logic: Keep only active pairs with valid numeric rates
                if parsed_item.rate is not None and parsed_item.status == "active":
                    valid_data.append((parsed_item.currency_pair, parsed_item.rate))
            except ValidationError as e:
                logging.warning(f"Data validation failed for record: {item}")
        
        logging.info(f"Transformation complete. {len(valid_data)}/{len(raw_data)} records are valid.")
        return valid_data

    def load(self, clean_data: List[tuple]):
        """Bulk inserts the cleaned data into the SQLite database."""
        logging.info("Loading data into SQLite database...")
        try:
            with sqlite3.connect(self.db_name) as conn:
                cursor = conn.cursor()
                cursor.executemany("""
                    INSERT INTO exchange_rates (pair, rate) VALUES (?, ?)
                """, clean_data)
                conn.commit()
            logging.info("Data successfully loaded.")
        except sqlite3.Error as e:
            logging.error(f"Database error during load: {e}")

    def run_pipeline(self):
        """Orchestrates the ETL process sequentially."""
        raw_data = self.extract()
        clean_data = self.transform(raw_data)
        if clean_data:
            self.load(clean_data)

    def analytics_summary(self):
        """Performs data aggregation using advanced SQL queries."""
        logging.info("Generating analytics summary...")
        with sqlite3.connect(self.db_name) as conn:
            cursor = conn.cursor()
            # SQL Query utilizing aggregation functions
            cursor.execute("""
                SELECT pair, AVG(rate), COUNT(*) 
                FROM exchange_rates 
                GROUP BY pair 
                ORDER BY AVG(rate) DESC
            """)
            results = cursor.fetchall()
            for row in results:
                logging.info(f"Parity: {row[0]} | Avg Rate: {row[1]:.2f} | Total Records: {row[2]}")

if __name__ == "__main__":
    etl = FinancialETL()
    etl.run_pipeline()
    etl.analytics_summary()