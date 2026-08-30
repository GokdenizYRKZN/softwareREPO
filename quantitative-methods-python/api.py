from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import numpy as np

# Yazdığın matematik modülünü içeri aktarıyoruz
from src.gauss_jordan_inverse import gauss_jordan_inv

app = FastAPI(title="Quantitative Analysis API", version="1.0")

# Dışarıdan gelecek JSON verisinin şemasını (N x N matris) belirliyoruz
class MatrixPayload(BaseModel):
    matrix: list[list[float]]

@app.post("/api/v1/math/matrix-inverse")
def calculate_inverse(data: MatrixPayload):
    # 1. Gelen listeyi NumPy dizisine çevir
    A = np.array(data.matrix)
    
    # 2. Kare matris kontrolü (Boyutlar eşit mi?)
    if A.shape[0] != A.shape[1]:
        raise HTTPException(status_code=400, detail="Matrix must be square (N x N).")
        
    try:
        # 3. Kendi yazdığın modüler fonksiyonu çalıştır
        inv_A = gauss_jordan_inv(A)
        
        # 4. Sonucu JSON formatına uygun hale getirip (listeye çevirip) döndür
        return {
            "method": "Gauss-Jordan Elimination with Partial Pivoting",
            "original_matrix": data.matrix,
            "inverse_matrix": inv_A.tolist()
        }
    except Exception as e:
        # Tekil (singular) matris hatası gibi durumları yakalayıp kullanıcıya ilet
        raise HTTPException(status_code=400, detail=str(e))