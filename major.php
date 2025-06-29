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

// pick a random note from the scale
$startNote = $scale[array_rand($scale)];

// pick a random interval
$interval = Scale::$intervalsArray[array_rand(Scale::$intervalsArray)];

// get the answer we're looking for (any of the acceptable notes)
$answer = $scale[$s->getMajorScalePosition($startNote, $interval, $scaleType)];

// echo "Start note is $startNote, scaletype is $scaleType, interval is $interval\n";
$acceptables = array($answer);

if (isset(Scale::$sames[$answer])) {
    $acceptables[] = Scale::$sames[$answer];
}

// print_r($acceptables);

do {
    // prompt the user for the note
    $users_answer = readline("In a $startNote major scale, what is the " . $interval . Scale::getIntervalEnding($interval) . " note? ");

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