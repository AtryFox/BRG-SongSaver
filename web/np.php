<?php
include "config.php";

if(!isset($_GET["key"])) {
    echo "error";
    exit;
}

if(KEY != $_GET["key"]) {
    echo "error";
    exit;
}

$rawJson = @file_get_contents("https://www.bronyradiogermany.com/request-v2/json/v1/nowplaying/stream");

if($rawJson === false) {
    echo "error";
    exit;
}

$currentSong = json_decode($rawJson);

if($currentSong->status != "success"){
    echo "error";
    exit();
}

$json = json_decode("{\"songs\": [] }");

if(file_exists("data.json")) {
    $json = json_decode(file_get_contents("data.json"));
}

$arr = $json->songs;

$currentSong->result->time = time();

array_push($arr, $currentSong->result);

$json = new stdClass();
$json->songs = $arr;

file_put_contents("data.json", json_encode($json));

echo "done";