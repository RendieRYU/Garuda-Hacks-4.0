<?php
include 'connect.php';
include ''

$username = $_POST['username'];
$password = $_POST['password'];

$check = mysqli_query($conn, "SELECT * from users where username = '$username' AND 
password ='$password'");

$same = mysqli_num_rows($check);

if($same > 0){
    session_start();
    $_SESSION['username'] = $username;
    $_SESSION['status'] =  'login';
    header("location: admin/index.php");
}
else{
    header("location: index.php?pesan=gagal");
}
?>