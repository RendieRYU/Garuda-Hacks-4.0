<?php

$libPath = 'D:/Pribadi/VsCode/gatau apaya/xampp/htdocs/hackathon/encrypt.dll';
// Memuat shared library menggunakan FFI
$ffi = \FFI::cdef("void encryptAES(const unsigned char *plainText, const unsigned char *key, unsigned char *cipherText);", $libPath);

$username = $_POST['username'];
// Memanggil fungsi C++ dari PHP

$plainText = $username;

$key = "admin";

$chipherText = FFI::new("unsigned char[16]");

$ffi->encryptAES($plainText, $key, $chipherText);

echo "Kata yang akan di enkripsi: " . $plainText . "\n";
    echo "Hasil Enkripsi: ";
    for ($i = 0; $i < 16; ++$i) {
        echo sprintf('%02x', $chipherText[$i]);
    }
    echo "\n";
?>
