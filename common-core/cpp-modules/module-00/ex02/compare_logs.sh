#/bin/sh

diff <(sed 's/^\[[^]]*\] //' 19920104_091532.log) <(sed 's/^\[[^]]*\] //' test.log)