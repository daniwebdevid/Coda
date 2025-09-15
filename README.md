Coda Build Tool

Coda adalah build tool sederhana yang dirancang untuk proyek C modular. Alat ini menggunakan pendekatan "Unity Build" untuk menyederhanakan proses kompilasi dan manajemen dependensi, sehingga Anda tidak perlu lagi berurusan dengan header atau makefile yang rumit.
Fitur Utama

    Modularitas Sederhana: Menggabungkan beberapa file .c menjadi satu file tunggal untuk dikompilasi, menghilangkan kebutuhan untuk file .h yang terpisah.

    Manajemen Dependensi Otomatis: Menggunakan coda install untuk mengunduh dependensi dari repositori Git, serta memperbarui file coda.json secara otomatis.

    Proses Build yang Mudah: Cukup jalankan satu perintah coda build untuk mengkompilasi seluruh proyek.

    Deteksi Perubahan Real-time (Experimental): Mengawasi file sumber untuk mendeteksi perubahan dan melakukan rebuild secara otomatis.

Persyaratan Sistem

Untuk membangun dan menggunakan Coda, Anda memerlukan:

    Linux: Proyek ini dirancang khusus untuk lingkungan Linux.

    Git: Diperlukan untuk mengelola dependensi.

    C Compiler: clang atau gcc.

    Library Jansson: Diperlukan untuk mem-parsing file JSON.

Untuk menginstal dependensi di sistem berbasis Debian/Ubuntu, gunakan perintah berikut:

sudo apt-get update
sudo apt-get install build-essential clang git libjansson-dev

Cara Membangun Coda

    Klon Repositori:

    git clone [https://github.com/nama-anda/coda.git](https://github.com/nama-anda/coda.git)
    cd coda

    Kompilasi dari Kode Sumber:
    Gunakan perintah clang untuk mengkompilasi tool Anda:

    clang src/main.c \
          src/registry_data.c \
          src/build_engine/build_engine.c \
          src/core_utils/core_utils.c \
          src/init_cmd/init_cmd.c \
          src/install_cmd/install_cmd.c \
          src/project_mgr/project_mgr.c \
          src/watch_cmd/watch_cmd.c \
          src/fs_monitor/fs_monitor.c \
          -o coda \
          -I./includes/ \
          -I./src/build_engine/ \
          -I./src/core_utils/ \
          -I./src/init_cmd/ \
          -I./src/install_cmd/ \
          -I./src/project_mgr/ \
          -I./src/watch_cmd/ \
          -I./src/fs_monitor/ \
          -ljansson \
          -Wall -Wextra

    (Opsional) Pindahkan ke PATH Anda:
    Untuk menjalankan coda dari direktori mana pun, pindahkan file executable ke direktori di PATH Anda.

    sudo mv coda /usr/local/bin/

Cara Menggunakan Coda

Setelah Anda membangun Coda, Anda dapat menggunakannya untuk proyek C baru.

    Inisialisasi Proyek Baru:

    coda init

    Ini akan membuat file coda.json di direktori proyek Anda.

    Instal Dependensi:

    coda install <nama_paket>

    Contoh: coda install rapidjson

    Bangun Proyek Anda:

    coda build

    Perintah ini akan membaca coda.json, mengkompilasi semua file sumber, dan menghasilkan executable di dist/.

Kontribusi

Proyek ini bersifat open-source. Kontribusi dalam bentuk bug reports, pull requests, atau ide-ide baru sangat dihargai.
Lisensi

Proyek ini dilisensikan di bawah [Tipe Lisensi] - lihat file LICENSE untuk detailnya.