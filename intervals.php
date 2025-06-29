#!/usr/bin/php
<?php
include 'include/config.inc';

$s = new Scale();

// pick flats or sharps
if (rand(0, 1)) {
    $scale = Scale::$flatsArray;
    $scaleType = Scale::$flats;
} else {
    $scale = Scale::$sharpsArray;
    $scaleType = Scale::$sharps;
}

if (rand(0, 1)) {
    $dir = Scale::$dirDown;
} else {
    $dir = Scale::$dirUp;
}

// pick a random note from the scale
$startNote = $scale[array_rand($scale)];

// pick a random number of semi-tones
$semitones = rand(1, 12);

// get the answers
$answer = $s->skipSemiTones($startNote, $semitones, $scaleType, $dir);

$acceptables = array($answer);

if (isset(Scale::$sames[$answer])) {
    $acceptables[] = Scale::$sames[$answer];
}

do {
    // prompt the user for the note
    $users_answer = readline("Starting on $startNote, count $semitones semi-tones " . strtolower($dir) . ": ");

    if ($users_answer == "q") {
        break;
    }

    // make the first character uppercase
    $users_answer[0] = strtoupper($users_answer[0]);

    // if there is a second character, see if it's a flat or a sharp and convert to unicode char
    if (strlen($users_answer) == 2) {
        if ($users_answer[1] == "b") {
            $users_answer = $users_answer[0] . Ascii::flat();
        } else if ($users_answer[1] == "#") {
            $users_answer = $users_answer[0] . Ascii::sharp();
        }
    }

    if (in_array($users_answer, $acceptables)) {
        echo "Thats right!  Way to go!\n";
        break;
    } else {
        echo "Incorrect.\n";
    }
} while(1);