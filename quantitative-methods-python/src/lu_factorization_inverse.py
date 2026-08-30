import numpy as np
from scipy.linalg import lu_factor, lu_solve

def mat_inv_lu(A):
    """
    A matrisinin tersini, LU ayrıştırmasını bir kez yapıp
    ardından N adet sistemi çözerek hesaplar (DLINEQ/DRESLV mantığı).
    """

    N = A.shape[0]

    # Ters matrisi tutacak N x N matrisi başlat (X = A^-1)
    X = np.zeros((N, N))

    # Sağ taraf vektörlerini (e_k) içeren birim matrisi oluştur
    I = np.eye(N)

    # 1. ADIM (DLINEQ'i Simüle Etme): LU Ayrıştırmasını hesapla
    # Bu, asıl pahalı olan (O(N^3)) adımdır.
    # lu_factor, (LU matrisi, pivot vektörü) döndürür
    lu, piv = lu_factor(A)
    print("--- 1. Adım: LU Ayrıştırması tamamlandı (DLINEQ) ---")

    # 2. ADIM (DRESLV'yi Simüle Etme): N adet sistemi çöz
    # Her bir sistem (A x_k = e_k) için lu_solve kullanılır.
    # Bu adım, her sütun için sadece ileri ve geri ikameyi (O(N^2)) uygular.
    for k in range(N):
        # I[:, k] birim matrisin k. sütunudur (e_k)
        x_k = lu_solve((lu, piv), I[:, k])
        X[:, k] = x_k

    print(f"--- 2. Adım: Kalan {N} sistem çözüldü (DRESLV) ---")

    return X

# Örnek Kullanım:
A_example = np.array([[25, 5, 1],
                      [64, 8, 1],
                      [144, 12, 1]], dtype=float)

A_inv = mat_inv_lu(A_example)

print("\nMatris A:")
print(A_example)
print("\nHesaplanan A^-1:")
print(A_inv)
print("\nKontrol (A * A^-1 - Birim Matris olmalı):")
print(A_example @ A_inv)
