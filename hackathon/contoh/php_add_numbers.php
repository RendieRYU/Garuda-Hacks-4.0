<?php
// Membaca shared library
    $libPath = 'D:/Pribadi/VsCode/gatau apaya/xampp/htdocs/hackathon/contoh/add_numbers.dll';

// Memuat shared library menggunakan FFI
$ffi = \FFI::cdef("int add_numbers(int a, int b);", $libPath);

// Memanggil fungsi C++ dari PHP
$result = $ffi->add_numbers(5, 10);
echo "Hasil penjumlahan: " . $result;
?>
