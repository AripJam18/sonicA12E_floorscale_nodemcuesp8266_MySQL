<?php
  //koneksi ke database
  $konek = mysqli_connect("localhost", "root", "", "websensor");

  //baca isi table loadcell
  $sql = mysqli_query($konek, "SELECT * FROM loadcell");
  $data = mysqli_fetch_array($sql);
  $i_weight = $data["weight"];

  //$nilai = round($i_weight*0.454, 1);   //convert from lbs to kg
    mysqli_query($konek, "INSERT INTO outscale (weight, unit) VALUES ('$i_weight', 'Kg')");

  //update data di database (table loadcell)
  mysqli_query($konek, "UPDATE loadcell SET weight=0, unit=''");

  header('Location: index.php');
