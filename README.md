Particle Simulation – Brute Force & QuadTree (C++)

Project ini adalah simulasi partikel saling bertabrakan menggunakan elastic collision.
Brute Force – setiap partikel ngecek semua partikel lain → lambat kalau jumlah partikel banyak
QuadTree – hanya cek objek yang berada di area sekitar → jauh lebih cepat untuk skala besar
Selain itu, QuadTree-nya bisa ditampilkan secara visual untuk melihat bagaimana partikel dibagi-bagi ke dalam node-node kecil.

Fitur Utama:
1. Gerakan partikel bebas dengan bouncing di tepi layar
2. Tabrakan antar partikel pakai rumus fisika beneran (elastic collision)
3. Dua mode perhitungan tabrakan:
   Brute Force – wajibnya O(N²),
   QuadTree – spatial partitioning

4. Toggle mode langsung dari keyboard
5. Double buffering → animasi lebih halus, nggak patah-patah
6. Tampilan QuadTree bisa dihidupkan/matikan
Tombol	Fungsi
Q	= Mode QuadTree
B	= Mode Brute Force
T	Menampilkan / menyembunyikan garis QuadTree

Cara Menjalankan:
1. Masukkan jumlah partikel (misal: 200)
2. Simulasi berjalan

Struktur Program Singkat:
1. Particle → posisi, kecepatan, radius, mass
2. AABB → bounding box untuk QuadTree
3. QuadTree → bagi layar jadi 4 bagian, simpan partikel sesuai region
4. resolveCollision() → hitung tumbukan antar partikel
5. main loop:
   update posisi,
   cek tabrakan (bruteforce atau quad),
   render,
   swap buffer

Kenapa Pakai QuadTree?
Kalau jumlah partikel makin banyak, metode bruteforce bakal jadi berat karena harus ngecek satu-satu:
N x N pengecekan.

Simulasi Quadtree
<img width="1135" height="909" alt="image" src="https://github.com/user-attachments/assets/430a5f5d-fe97-419a-9b33-4b75c140f303" />
https://github.com/user-attachments/assets/9e18a92d-c0f3-4823-a39f-52e83f57be6d

Simulasi Bruteforce
<img width="1129" height="910" alt="Image" src="https://github.com/user-attachments/assets/4f7f84aa-dfdc-456b-889d-afbdab728e24" />
https://github.com/user-attachments/assets/bf0f6dcb-af70-4a24-bd05-9cc6f9d92eb3
Dengan QuadTree, program cuma ngecek partikel yang deket-deket aja, bukan seluruh layar.
Hasilnya, fps tetap stabil meskipun partikel banyak.
