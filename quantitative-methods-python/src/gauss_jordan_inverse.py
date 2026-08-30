import numpy as np

def gauss_jordan_inv(A):
    """
    Gauss-Jordan eliminasyon yöntemini kullanarak A matrisinin tersini hesaplar.

    Args:
        A (numpy.array): Tersi alınacak N x N matris.

    Returns:
        numpy.array: A'nın tersi olan A^-1 matrisi.
    """

    # Giriş matrisinin kopyasını al ve N x N boyutunu kontrol et
    A_copy = A.copy().astype(float)
    N = A_copy.shape[0]

    # 1. Adım: Artırılmış matris [A | I] oluştur
    I = np.eye(N)
    AUG = np.hstack((A_copy, I))

    print("--- 5. Gauss-Jordan Eliminasyonu Başlatılıyor ---")

    # Eliminasyon Aşaması
    for k in range(N):
        # 2. Adım: Kısmi Pivotlama (Sayısal kararlılık için)
        # k. sütunun k. elemanından N. elemanına kadar olan kısımda en büyük mutlak değeri bul
        pivot_col = AUG[k:, k]
        pivot_idx = np.argmax(np.abs(pivot_col))
        pivot_row = k + pivot_idx

        if AUG[pivot_row, k] == 0:
            raise RuntimeError("Matris tekil (singular). Tersi mevcut değil.")

        # Satır değişimi
        AUG[[k, pivot_row]] = AUG[[pivot_row, k]]

        # 3. Adım: Normalize Etme (Pivot elemanını 1 yap)
        pivot = AUG[k, k]
        AUG[k, :] = AUG[k, :] / pivot

        # 4. Adım: Eleme (Hem ileri hem de geri eleme)
        for i in range(N):
            if i != k:
                factor = AUG[i, k]
                # AUG(i, :) = AUG(i, :) - factor * AUG(k, :)
                AUG[i, :] = AUG[i, :] - factor * AUG[k, :]

        # print(f"\nAdım k={k+1} sonrası AUG:")
        # print(AUG)

    # Ters matris X, AUG'nin sağ yarısıdır
    X = AUG[:, N:]

    return X

# Sadece bu dosya terminalden doğrudan çalıştırıldığında test kodlarını tetikle,
# API gibi başka bir yere import edildiğinde bu kısmı atla.
if __name__ == "__main__":
    A_test = np.array([[3, -2, 2],
                       [1, 0, 1],
                       [-4, -4, 4]])

    inv_A = gauss_jordan_inv(A_test)

    print("\nGauss-Jordan ile Hesaplanan A^-1:")
    print(inv_A)