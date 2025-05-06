# 🧩 Sudoku

![Podgląd gry](assets/AnimationSudoku.gif) <!-- ← ścieżka do gifa; jeśli masz inną, podmień -->


* Rozmiary planszy **4 × 4**, **9 × 9** oraz **16 × 16**  
* Generator z back-trackingiem 
* Walidacja ruchu w czasie rzeczywistym  
* Polecenia `save` / `load` zapisują postęp do **save.txt**

---

## ✨ Zrealizowane funkcje

| Kategoria          | ✔ |
|--------------------|--- |
| Rozmiary planszy   | ✅ |
| Generator          | ✅ | 
| Walidacja ruchu    | ✅ | 
| Zapisywanie / load | ✅ | 
| Obsługa ruchu      | ✅ | 

---

## 🛠️ TODO 
## Te sudoku śni i będzie mi się śnić po nocach
(Szanowny Panie Łukaszu chciałbym tylko to zaliczyć)

| Plan                             | Status | Uwagi                                               |
|----------------------------------|--------|-----------------------------------------------------|
| Cofanie / czyszczenie ruchu      | ❌     |                                                     |
| Stoper + licznik ruchów          | ❌     |                                                     |
| Gwarancja jednego rozwiązania    | ❌     | Generator sprawdza poprawność, lecz nie unikalność  |

---

## Meme of the month
![Meme](assets/Meme.jpg)

## ⏳ Kompilacja i uruchomienie
### MinGW / GCC – najszybciej

```bash
gcc *.c -std=c11 -Wall -O2 -o sudoku.exe
./sudoku.exe

