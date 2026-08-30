# Quantitative Analysis & REST API Microservice

A production-ready Python microservice that bridges the gap between raw numerical analysis and scalable software engineering. This repository contains both from-scratch mathematical implementations and a fully dockerized REST API designed to serve these models.

**Core Architecture & Features**
* **RESTful API Endpoint:** Built with FastAPI to expose a matrix inversion service using a custom Gauss-Jordan elimination algorithm.
* **Containerization:** Fully containerized using Docker, ensuring isolated, consistent deployment.
* **Linear Algebra Solvers:** Custom Python implementations of Gauss-Jordan elimination with partial pivoting and LU Factorization.
* **Numerical Analysis Notebooks:** Comprehensive Jupyter notebooks detailing root finding algorithms and ODE solvers.

**Tech Stack**
* **Backend:** Python 3, FastAPI, Uvicorn
* **DevOps:** Docker
* **Data & Math:** NumPy, SciPy, Matplotlib

**How to Run via Docker**
1. Clone the repository and build the image:
git clone https://github.com/yourusername/quantitative-api.git
cd quantitative-api
docker build -t quantitative-api .

2. Run the container:
docker run -d -p 8000:8000 quantitative-api

3. Access the interactive Swagger UI at: http://localhost:8000/docs