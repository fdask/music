#!/usr/bin/php
<?php
include 'include/music.inc';

$strings = array("E", "A", "D", "G", "B", "E");
$positions = array(1, 2, 3, 5, 6);
$shapes = array("C", "A", "G", "E", "D");
$fret = rand(0, 11);

$string = $strings[array_rand($strings)];
$shape = $shapes[array_rand($shapes)];

echo "Fret $fret on the $string string.\n";
echo "$shape shape.  So either a "

