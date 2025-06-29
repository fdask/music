#!/usr/bin/php
<?php
$caged = array('C', 'A', 'G', 'E', 'D');
$key_flats = array('A', 'Bb', 'B', 'C', 'Db', 'D', 'Eb', 'E', 'F', 'Gb', 'G', 'Ab');
$key_sharps = array('A', 'A#', 'B', 'C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#');

$pos = $caged[array_rand($caged)];

if (rand(0, 1)) {
    $tmp = $key_flats;
} else {
    $tmp = $key_sharps;
}

$key = $tmp[array_rand($tmp)];

echo "Key of $key, $pos position.\n";