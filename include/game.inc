<?php
class Game {
    private $term;
    private $tab;
    private $scale;
    private $timer;
    private $record;

    private $guess;
    
    private $flat;
    private $sharp;

    public function __construct() {
        $this->init();

        $this->term = `stty -g`;
		system("stty -icanon -echo");

        //stream_set_blocking(STDIN, false); // Do not wait

        $this->gameLoop();
    }

    public function init() {
        $minutes = null;

        while (is_null($minutes) || !is_int($minutes) || $minutes == 0) {
            echo "\n";
            $minutes = (int)readline("How many minutes do you wish to train? ");
        }

        $duration = $minutes * 60;

        $this->tab = new Tab(6, 12);
        $this->scale = new Scale();
        $this->timer = new Timer();
        $this->record = new Record($duration);

        $this->reset();
    }
    
    private function reset() {
        // clear the note
        $this->tab->clearNotes();

        // add a new random note
        $this->tab->addRandomNote();

        // clear the timer
        $this->timer->reset();

        // clear the guess
        $this->guess = null;

        // clear the flats/sharps
        $this->flat = false;
        $this->sharp = false;
    }

    private function sharp() {
        if ($this->flat) {
            $this->flat = false;
            $this->sharp = false;
        } else if (!$this->flat && !$this->sharp) {
            $this->sharp = true;
            $this->flat = false;
        }   
    }

    private function flat() {
        if ($this->sharp) {
            $this->sharp = false;
            $this->flat = false;
        } else if (!$this->flat && !$this->sharp) {
            $this->flat = true;
            $this->sharp = false;
        }
    }

    public function getGuess() {
        $guess = $this->guess;

        if ($this->flat) {
            $guess .= Ascii::flat();
        } else if ($this->sharp) {
            $guess .= Ascii::sharp();
        }

        return $guess;
    }

    public function guess() {
        // what note are we actually on
        $notes = $this->tab->getNotes();

        $secs = $this->timer->getTime();

        $strings = array_keys($notes);

        if (count($strings) == 1) {
            $frets = array_values($notes[$strings[0]]);

            $scale = null;

            if (count($frets) == 1) {
                if ($this->sharp) {
                    $scale = Scale::$sharps;
                } else if ($this->flat) {
                    $scale = Scale::$flats;
                }

                $guess = $this->getGuess();
                $note = $this->scale->getNote($strings[0], $frets[0], $scale);

                $allValid = array($note);

                if (isset(Scale::$sames[$note])) {
                    $allValid[] = Scale::$sames[$note];
                }

                sort($allValid);

                echo "\n";

                // see if our time has expired
                $secs = time() - $this->record->getStartTime();
                $duration = $this->record->getDuration();

                if ($secs > $duration) {
                    $this->gameOver();
                }

                if (in_array($guess, $allValid)) {
                    echo "YOU GUESSED RIGHT!";
                    $this->record->addRight($secs);
                } else {
                    echo "YOU GUESSED WRONG! (Could have been " . implode(", ", $allValid) . ")";
                    $this->record->addWrong($secs);
                }
            }
        }

        echo " ($secs seconds)\n";

        $this->reset();
    }

    public function gameLoop() {
        echo $this;

        while (1) {
            $redraw = true;

            $c = Config::waitForInput();

            switch ($c) {
                case 'a':
                case 'A':
                    $this->guess = "A";

                    break;
                case 'b':
                case 'B':
                    $this->guess = "B";

                    if ($this->sharp) {
                        $this->sharp = false;
                        $this->flat = false;
                    }

                    break;
                case 'c':
                case 'C':
                    $this->guess = "C";

                    if ($this->flat) {
                        $this->flat = false;
                        $this->sharp = false;
                    }

                    break;
                case 'd':
                case 'D':
                    $this->guess = "D";

                    break;
                case 'e':
                case 'E':
                    $this->guess = "E";

                    if ($this->sharp) {
                        $this->sharp = false;
                        $this->flat = false;
                    }

                    break;
                case 'f':
                case 'F':
                    $this->guess = "F";

                    if ($this->flat) {
                        $this->flat = false;
                        $this->sharp = false;
                    }

                    break;
                case 'g':
                case 'G':
                    $this->guess = "G";

                    break;
                case 'q':
                    $this->gameOver();

                    break;
                case Keypress::backspace():
                    $this->guess = null;

                    break;
                case Keypress::leftArrow():
                    $this->flat();

                    break;
                case Keypress::rightArrow():
                    $this->sharp();

                    break;
                case Keypress::enter():
                    $this->guess();

                    break;
                default:
                    // unhandled keypress
                    if ($c != '') {
                        echo "unhandled keypress: ";
                        print_r(Config::strToChr($c));
                    }
            }

            if ($redraw) {
                echo $this;
            }
        }

        $this->gameOver();
    }

    public function gameOver() {
        system("stty " . $this->term);

        echo "\n";

        $this->record->stop();

        exit;
    }

    public function __toString() {
        $ret = "\n\n";

        $ret .= "Score: " . $this->record->getScore() . " Time Avg: " . $this->record->getAverageTime() . "\n";
        $ret .= $this->tab;
        $ret .= "Guess: " . $this->guess;

        if ($this->flat) {
            $ret .= Ascii::flat();
        } else if ($this->sharp) {
            $ret .= Ascii::sharp();
        }
        
        return $ret;
    }
}