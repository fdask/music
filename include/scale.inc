<?php
class Scale {
    public static $majorIntervals = array(
        'W',
        'W',
        'H',
        'W',
        'W',
        'W',
        'H'
    );

    public static $intervalsArray = array(
        2,
        3,
        4,
        5,
        6,
        7
    );

    public static $flatsArray = array(
        'A♭',
        'A',
        'B♭',
        'B',
        'C',
        'D♭',
        'D',
        'E♭',
        'E',
        'F', 
        'G♭',
        'G'
    );

    public static $sharpsArray = array(
        'A',
        'A♯',
        'B',
        'C',
        'C♯',
        'D',
        'D♯',
        'E',
        'F',
        'F♯',
        'G',
        'G♯'
    );

    public static $sames = array(
        'A♯' => 'B♭',
        'B♯' => 'C',
        'C♯' => 'D♭',
        'D♯' => 'E♭',
        'E♯' => 'F',
        'G♯' => 'A♭',
        'C' => 'B♯',
        'E' => 'F♭',
        'F' => 'E♯',
        'A♭' => 'G♯',
        'B♭' => 'A♯',
        'C♭' => 'B',
        'D♭' => 'C♯',
        'E♭' => 'D♯',
        'F♭' => 'E',
        'G♭' => 'F♯'
    );

    public static $flats = "FLATS";
    public static $sharps = "SHARPS";
    public static $dirUp = "UP";
    public static $dirDown = "DOWN";

    /**
     * given a root, and a position number, return the note
     * 
     * @param string $root the starting note, alphabetical
     * @param int $position what position of the scale do you want?
     * @param null|string $flatsOrSharps is the scale using flats or sharps
     */
    public function getMajorScalePosition($root, $position, $flatsOrSharps = null) {
        $startPos = $this->getPosInArray($root, $flatsOrSharps);
        $endPos = $startPos;

        // now calculate how far we have to move
        for ($x = 2; $x <= $position; $x++) {
            $interval = Scale::$majorIntervals[$x - 2];

            if ($interval == "W") {
                $endPos += 2;        
            } else if ($interval == "H") {
                $endPos += 1;
            }
        }

        // rotate
        $x = $this->rotate($startPos, $flatsOrSharps, Scale::$dirUp, $endPos - $startPos);

        return $x;
    }

    public function skipSemiTones($startNote, $count = 0, $flatsOrSharps = null, $direction = null) {
        if (is_null($flatsOrSharps) || $flatsOrSharps == Scale::$flats) {
            $scale = Scale::$flatsArray;
        } else {
            $scale = Scale::$sharpsArray;
        }

        if (is_null($direction)) {
            $direction = Scale::$dirUp;
        } 

        $startPos = $this->getPosInArray($startNote, $flatsOrSharps); 

        $endPos = $this->rotate($startPos, $flatsOrSharps, $direction, $count);

        return $scale[$endPos];
    }

    public function rotate($i, $flatsOrSharps = null, $direction = null, $count = 0) {
        if (is_null($flatsOrSharps) || $flatsOrSharps == Scale::$flats) {
            $scale = Scale::$flatsArray;
        } else {
            $scale = Scale::$sharpsArray;
        }

        $startPos = $i;

        for ($c = 0; $c < $count; $c++) {
            if ($direction == Scale::$dirUp || is_null($direction)) {
                $startPos++;

                if ($startPos > (count($scale) - 1)) {
                    $startPos = 0;
                }
            } else if ($direction == Scale::$dirDown) {
                $startPos--;

                if ($startPos < 0) {
                    $startPos = count($scale) - 1;
                }
            }
        } 

        return $startPos;
    }

    /**
     * finds a note in the given array
     */
    public function getPosInArray($value, $flatsOrSharps = null) { 
        $scale = ($flatsOrSharps == Scale::$flats || $flatsOrSharps == null) ? Scale::$flatsArray : Scale::$sharpsArray;
        $pos = array_search($value, $scale);

        return $pos;
    }

    public function getNote($string, $fret, $flatsOrSharps = null) {
        if (is_null($flatsOrSharps)) {
            $flatsOrSharps = Scale::$flats;
        }

        switch ($string) {
            case 0:
            case 5:
                $start = "E";

                break;
            case 1:
                $start = "B";

                break;
            case 2:
                $start = "G";

                break;
            case 3:
                $start = "D";

                break;
            case 4:
                $start = "A";

                break;
        }

        $pos = $this->getPosInArray($start, $flatsOrSharps);
        
        $newPos = $this->rotate($pos, $flatsOrSharps, Scale::$dirUp, $fret);

        if ($flatsOrSharps == Scale::$flats || is_null($flatsOrSharps)) {
            $scale = Scale::$flatsArray;
        } else {
            $scale = Scale::$sharpsArray;
        }

        return $scale[$newPos];
    }

    public static function getIntervalEnding($int) {
        switch ($int) {
            case 2:
                return "nd";
                break;
            case 3:
                return "rd";
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                return "th";
                break;
        }
    }
}