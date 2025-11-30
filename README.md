Particle Simulation – Brute Force & QuadTree (C++)

Project ini adalah simulasi partikel saling bertabrakan menggunakan elastic collision.
Brute Force – setiap partikel ngecek semua partikel lain → lambat kalau jumlah partikel banyak
QuadTree – hanya cek objek yang berada di area sekitar → jauh lebih cepat untuk skala besar
Selain itu, QuadTree-nya bisa ditampilkan secara visual untuk melihat bagaimana partikel dibagi-bagi ke dalam node-node kecil.

Fitur Utama:
1. Gerakan partikel bebas dengan bouncing di tepi layar
2. Tabrakan antar partikel pakai rumus fisika beneran (elastic collision)
3. Dua mode perhitungan tabrakan:
   Brute Force – wajibnya O(N²)
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
   update posisi
   cek tabrakan (bruteforce atau quad)
   render
   swap buffer

Kenapa Pakai QuadTree?
Kalau jumlah partikel makin banyak, metode bruteforce bakal jadi berat karena harus ngecek satu-satu:
N x N pengecekan.

Dengan QuadTree, program cuma ngecek partikel yang deket-deket aja, bukan seluruh layar.
Hasilnya, fps tetap stabil meskipun partikel banyak.
