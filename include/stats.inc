<?php
class Stats {
    // each record, is a Record object.  
    // stats for a single game
    private $records;

    // holds all full games
    private $fulls;

    // holds all partial games
    private $partials;

    // total amount of seconds spent in game
    private $totalSeconds;

    // highest score out of all games
    private $scoreHigh;

    // lowest score out of all games
    private $scoreLow;

    // average score out of all games
    private $scoreAvg;

    // total number of right answers
    private $rightCount;

    // total number of wrong answers
    private $wrongCount;

    // total number of answers
    private $totalCount;

    // average time to answer a question across all games
    private $timeAvg;

    // average time to answer correctly across all games
    private $timeRightAvg;

    // average time to answer incorrectly across all games
    private $timeWrongAvg;

    // average game duration in seconds
    private $durationAvg;

    // score as a percentage average per game
    private $perGameAvg;

    public function __construct() {
        $this->init();
        $this->load();
    }

    private function init() {
        $this->records = array();
       
        $this->fulls = array();
        $this->partials = array();

        $this->totalSeconds = 0;

        $this->scoreHigh = 0;
        $this->scoreLow = 0;
        $this->scoreAvg = 0;

        $this->rightCount = 0;
        $this->wrongCount = 0;
        $this->totalCount = 0;

        $this->timeAvg = 0;
        $this->timeRightAvg = 0;
        $this->timeWrongAvg = 0;

        $this->durationAvg = 0;

        $this->perGameAvg = 0;
    }

    private function load() {
        $files = scandir(Config::$saveDir);

        $this->records = array();

        foreach ($files as $file) {
            if (preg_match("@\d+@", $file)) {
                $this->records[] = unserialize(file_get_contents(Config::$saveDir . "/" . $file));
            }
        }

        $this->recalc();
    }

    public function recalc() {
        // initialize vars
        $scores = array();

        $avgTotals = array();
        $avgRights = array();
        $avgWrongs = array();
        $durations = array();
        $gamePcts = array();
        
        $this->fulls = array();
        $this->partials = array();

        $this->totalSeconds = 0;
        $this->rightCount = 0;
        $this->wrongCount = 0;
        $this->totalCount = 0;

        // loop over each record
        foreach ($this->records as $record) {
            // calculate the total duration of all records
            $this->totalSeconds += $record->getDuration();

            if ($record->getFullDuration()) {
                $this->fulls[] = $record;

                // only consider the times of full games
                $rights = $record->getRights();
                $wrongs = $record->getWrongs();

                $this->rightCount += count($rights);
                $this->wrongCount += count($wrongs);
                $this->totalCount += count($rights) + count($wrongs);

                $avg = 0;

                if (count($rights)) {
                    $avgRights[] = array_sum($rights) / count($rights);
                    $avg += array_sum($rights) / count($rights);
                } 
                            
                if (count($wrongs)) {
                    $avgWrongs[] = array_sum($wrongs) / count($wrongs);
                    $avg += array_sum($wrongs) / count($wrongs);
                }
        
                // only consider the scores of complete durations
                $scores[] = $record->getScore();
            } else {
                $this->partials[] = $record;
            }

            $durations[] = $record->getDuration();
            $gamePcts[] = (count($rights) / (count($rights) + count($wrongs))) * 100;

            if (count($rights) && count($wrongs)) {
                $avgTotals[] = $avg / 2;
            } else if (count($rights) || count($wrongs)) {
                $avgTotals[] = $avg;
            } 
        }

        // now calculate the grand total of all records
        if (count($gamePcts)) {
            $this->perGameAvg = round(array_sum($gamePcts) / count($gamePcts), 2);
        }

        if (count($scores)) {
            $this->scoreHigh = max($scores);
            $this->scoreLow = min($scores);
            $this->scoreAvg = round(array_sum($scores) / count($this->fulls), 2);
        }

        if (count($avgTotals)) {
            $this->timeAvg = round(array_sum($avgTotals) / count($avgTotals), 2);
        }

        if (count($avgRights)) {
            $this->timeRightAvg = round(array_sum($avgRights) / count($avgRights), 2);
        }

        if (count($avgWrongs)) {
            $this->timeWrongAvg = round(array_sum($avgWrongs) / count($avgWrongs), 2);
        }

        if (count($durations)) {
            $this->durationAvg = array_sum($durations) / count($durations);
        }
    }

    private function returnTime($seconds) {
        return round($seconds / 60, 0) . ":" . str_pad($seconds % 60, "2", "0", STR_PAD_LEFT); 
    }

    public function compare($record) {
        $ret = array();

        $score = $record->getScore();

        if ($score > $this->scoreHigh) {
            $ret[] = "NEW HIGH SCORE!";
        } else if ($score < $this->scoreLow) {
            $ret[] = "NEW low score.";
        } else if ($score > $this->scoreAvg) {
            $ret[] = "Better than average score.";
        } else if ($score < $this->scoreAvg) {
            $ret[] = "Bringing down the average.";
        }

        return $ret;
    }

    public function addRecord($r) {
        $this->records[] = $r;
    }

    public function getFulls() {
        return $this->fulls;
    }

    public function getPartials() {
        return $this->partials;
    }

    public function timeDiff($old, $new) {
        $ret = "";

        if ($old > $new) {
            $secs = $old - $new;

            if ($secs >= 1) {
                $ret .= " (-" . Colors::fg_color('red', $this->returnTime($old - $new)) . ")";
            }
        } else if ($old < $new) {
            $secs = $new - $old;

            if ($secs >= 1) {
                $ret .= " (+" . Colors::fg_color('green', $this->returnTime($new - $old)) . ")";
            }
        }

        return $ret;
    }

    public function percentDiff($old, $new, $rev = false) {
        $ret = "";

        if ($rev) {
            $low = "green";
            $high = "red";
        } else {
            $low = "red";
            $high = "green";
        }

        if ($old > $new) {
            $ret .= " (-" . Colors::fg_color($low, round($old - $new, 2)) . ")";
        } else if ($old < $new) {
            $ret .= " (+" . Colors::fg_color($high, round($new - $old, 2)) . ")";    
        } 

        return $ret;
    }

    public function getRightCount() {
        return $this->rightCount;
    }

    public function getWrongCount() {
        return $this->wrongCount;
    }

    public function getTotalCount() {
        return $this->totalCount;
    }

    public function getPerGameAverage() {
        return $this->perGameAvg;
    }

    public function getTotalSeconds() {
        return $this->totalSeconds;
    }

    public function getDurationAverage() {
        return $this->durationAvg;
    }

    public function getTimeAverage() {
        return $this->timeAvg;
    }

    public function getTimeRightAverage() {
        return $this->timeRightAvg;
    }

    public function getTimeWrongAverage() {
        return $this->timeWrongAvg;
    }

    public function getHighScore() {
        return $this->scoreHigh;
    }

    public function getLowScore() {
        return $this->scoreLow;
    }

    public function getAverageScore() {
        return $this->scoreAvg;
    }

    public function getStats($b = null) {
        $ret = "";

        $aatp =  round(($this->rightCount / $this->totalCount) * 100, 2);

        // count of full and partial games
        if (!is_null($b)) {
            $bbtp = round(($b->getRightCount() / $b->getTotalCount()) * 100, 2);

            if (count($this->fulls) > count($b->getFulls())) {
                $fulls = Colors::fg_color('green', count($this->fulls));
            } else {
                $fulls = count($this->fulls);
            }

            if (count($this->partials) > count($b->getPartials())) {
                $partials = Colors::fg_color('red', count($this->partials));
            } else {
                $partials = count($this->partials);
            }

            $total_count = $this->totalCount . $this->percentDiff($b->getTotalCount(), $this->totalCount);
            $right_count = $this->rightCount . $this->percentDiff($b->getRightCount(), $this->rightCount);
            $wrong_count = $this->wrongCount . $this->percentDiff($b->getWrongCount(), $this->wrongCount);

            $atp = "$aatp%" . $this->percentDiff($aatp, $bbtp);
            $pga = round($this->perGameAvg, 2) . "%" . $this->percentDiff($b->getPerGameAverage(), $this->perGameAvg);

            $total_seconds = $this->returnTime($this->totalSeconds) . $this->timeDiff($b->getTotalSeconds(), $this->totalSeconds);
            $average_length = $this->returnTime($this->durationAvg) . $this->timeDiff($b->getDurationAverage(), $this->durationAvg);
            $average_answer_time = $this->timeAvg . $this->percentDiff($b->getTimeAverage(), $this->timeAvg, true);
            $average_answer_time_right = $this->timeRightAvg . $this->percentDiff($b->getTimeRightAverage(), $this->timeRightAvg, true);
            $average_answer_time_wrong = $this->timeWrongAvg . $this->percentDiff($b->getTimeWrongAverage(), $this->timeWrongAvg, true);

            if ($b->getHighScore() > $this->scoreHigh) {
                $high_score = Colors::fg_color('green', $b->getHighScore());
            } else {
                $high_score = $this->scoreHigh;
            }

            if ($b->getLowScore() < $this->scoreLow) {
                $low_score = Colors::fg_color('red', $b->getLowScore());
            } else {
                $low_score = $this->scoreLow;
            }

            $avg_score = $this->scoreAvg . $this->percentDiff($b->getAverageScore(), $this->scoreAvg);
        } else {
            $fulls = count($this->fulls);
            $partials = count($this->partials);
            $total_count = $this->totalCount;
            $right_count = $this->rightCount;
            $wrong_count = $this->wrongCount;
            $atp = "$aatp%";
            $pga = round($this->perGameAvg, 2) . "%";
            $total_seconds = $this->returnTime($this->totalSeconds);
            $average_length = $this->returnTime($this->durationAvg);
            $average_answer_time = $this->timeAvg;
            $average_answer_time_right = $this->timeRightAvg;
            $average_answer_time_wrong = $this->timeWrongAvg;
            $high_score = $this->scoreHigh;
            $low_score = $this->scoreLow;
            $avg_score = $this->scoreAvg;
        }
            
        $ret .= "$fulls full games, $partials partial games.\n";

        // total rights/wrongs
        $ret .= "$right_count correct, out of $total_count answered.  (Wrongs: $wrong_count)\n";

        // all time percentage
        $ret .= "Your all time percentage is $atp with an average per game of $pga.\n";

        // total training time
        $ret .= "You have trained for a total of $total_seconds! (Average length is $average_length) \n";

        // average answer time
        $ret .= "Your average answer time was $average_answer_time seconds. ";
        
        $ret .= "(";

        if ($this->timeRightAvg != 0) {
            $ret .= "$average_answer_time_right right";
        }
        
        if ($this->timeRightAvg != 0 && $this->timeWrongAvg != 0) {
            $ret .= ", $average_answer_time_wrong wrong";
        } else if ($this->timeWrongAvg != 0) {
            $ret .= "$average_answer_time_wrong wrong";
        }

        $ret .= ")\n";

        // highest score, lowest score, average score
        $ret .= "High Score: $high_score Low Score: $low_score Average Score: $avg_score.\n";

        return $ret;
    }
}