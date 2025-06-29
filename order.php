#!/usr/bin/php
<?php

include 'include/music.inc';

// pick any 5 random notes from the set of [1, 2, 3, 4, 5, 6, 7]
$notes = array(1, 2, 3, 4, 5, 6, 7);
$count = 10;
$repeats = 2;
$ints = array();

$out = Music::getNotes($notes, $count, $repeats);

for ($x = 1; $x < count($out); $x++) {
    $a = $out[$x - 1];
    $b = $out[$x];

    $ints[] = Music::getInterval($a, $b);
}

//echo implode(", ", $ints) . "\n";

echo "Key is " . $key . "\n";
echo "Positions are " . implode(", ", $out) . "\n";
echo "Intervals are " . implode(", ", $ints) . "\n";


