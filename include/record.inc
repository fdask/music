<?php
class Record {
    public $startTime;
    public $duration;
    public $score;
    public $rights;
    public $wrongs;
    public $fullDuration;

    public function __construct($duration) {
        $this->score = 0;
        $this->startTime = time();
        $this->duration = $duration;
        $this->rights = array();
        $this->wrongs = array();
        $this->fullDuration = false;

        if (!file_exists(Config::$saveDir)) {
            mkdir(Config::$saveDir);
        }
    }

    public function addRight($time) {
        $this->score++;

        $this->rights[] = $time;
    }

    public function addWrong($time) {
        $this->wrongs[] = $time;
    }

    public function getDuration() {
        return $this->duration;
    }

    public function getScore() {
        return $this->score;
    }

    public function getStartTime() {
        return $this->startTime;
    }

    public function stop() {
        if ((time() - $this->startTime) > $this->duration) {
            echo "\nTime is up!\n";
            $this->fullDuration = true;
        } else {
            $this->fullDuration = false;
            $this->setDuration();
        }

        $this->save();
    }

    public function getStr() {
        $won = count($this->rights);
        $lost = count($this->wrongs);

        echo "Got $won/$lost wins/losses.\n";
        echo "Average time was " . $this->getAverageTime() . " seconds.\n";
    }

    public function save() {
        $filename = Config::$saveDir . "/" . $this->startTime;

        if (file_put_contents($filename, serialize($this))) {
            echo "\nResults saved to '$filename'\n";
        }
    }

    public function getRights() {
        return $this->rights;
    }

    public function getWrongs() {
        return $this->wrongs;
    }

    public function getAverageTime() {
        $times = array_merge($this->rights, $this->wrongs);
        
        if (count($times) > 0) {
            return round(array_sum($times) / count($times), 3);
        }

        return 0;
    }

    public function getAverageTimeRights() {
        if (count($this->rights) > 0) {
            return round(array_sum($this->rights) / count($this->rights), 3);
        }

        return 0;
    }

    public function getAverageTimeWrongs() {
        if (count($this->wrongs) > 0) {
            return round(array_sum($this->wrongs) / count($this->wrongs), 3);
        }

        return 0;
    }

    public function getFullDuration() {
        return $this->fullDuration;
    }

    public function setDuration() {
        $this->duration = time() - $this->startTime;
    }
}