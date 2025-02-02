<?php
class Tab {
    private $stringCount;
    private $strings;
    private $fretMax;

    public function __construct($strings = 6, $frets = 12) {
        // string 0 is high e
        // string 5 is low E
        // internally, we use 0-5.

        $this->stringCount = $strings;
        $this->fretMax = $frets;

        // start with blank notes
        $this->clearNotes();
    }

    public function addRandomNote() {
        $string = rand(1, $this->stringCount);
        $fret = rand(0, $this->fretMax);

        $this->addNote($string, $fret);
    }

    private function addNote($string, $fret) {
        $this->strings[$string - 1][$fret] = "X";
    }

    public function clearNotes() {
        for ($x = 0; $x < $this->stringCount; $x++) {
            $this->strings[$x] = array();
        }
    }

    public function getNotes() {
        $ret = array();

        for ($x = 0; $x < $this->stringCount; $x++) {
            $frets = array_keys($this->strings[$x]);

            if (count($frets)) {
                foreach ($frets as $fret) {
                    if (isset($this->strings[$x][$fret])) {
                        $ret[$x][] = $fret;
                    }
                }
            }
        }

        return $ret;
    }

    /**
     * there is either a mark on the given fret (width 3),
     * or there is not (width 2)
     */
    private function getFretWidth($fret) {
        $ret = 3;

        for ($x = 0; $x < $this->stringCount; $x++) {
            if (!empty($this->strings[$x][$fret])) {
                $ret = strlen($this->strings[$x][$fret]) + 2;

                break;
            }
        }

        if ($fret == 0) {
            $ret = $ret - 2;
        }

        return $ret;
    }

    public function __toString() {
        $draw = array();

        // draw zero fret
        $zeroFretWidth = $this->getFretWidth(0);

        for ($string = 0; $string < $this->stringCount; $string++) {
            $draw[$string] = "";

            if (!empty($this->strings[$string][0])) {
                $draw[$string] .= str_pad($this->strings[$string][0], $zeroFretWidth, " ", STR_PAD_LEFT);
            } else {
                $draw[$string] .= str_pad("", $zeroFretWidth, " ", STR_PAD_LEFT);
            }

            $draw[$string] .= "||";
        }

        $draw[$this->stringCount] = str_pad("", $zeroFretWidth + 2, " ", STR_PAD_LEFT);

        // now do the rest of the frets
        for ($fret = 1; $fret <= $this->fretMax; $fret++) {
            $fretWidth = $this->getFretWidth($fret);

            for ($string = 0; $string < $this->stringCount; $string++) {
                if (!empty($this->strings[$string][$fret])) {
                    $draw[$string] .= str_pad($this->strings[$string][$fret], $fretWidth, "-", STR_PAD_BOTH);
                } else {
                    $draw[$string] .= str_pad("", $fretWidth, "-", STR_PAD_BOTH);
                }

                $draw[$string] .= "|";
            }

            if (in_array($fret, array(3, 5, 7, 9, 12, 15, 17, 19, 21, 24))) {
                $draw[$this->stringCount] .= str_pad("o", $fretWidth + 1, " ", STR_PAD_BOTH);
            } else {
                $draw[$this->stringCount] .= str_pad("", $fretWidth + 1, " ", STR_PAD_LEFT);
            }
        }

        // now draw the fret markers
        $ret = implode("\n", $draw) . "\n";

        return $ret;
    }
}