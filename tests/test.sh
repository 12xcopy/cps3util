rm -rf ./combo
rm -rf ./split
rm -rf ./hash.txt
../cpsutil -combine -i ../working/jojoba.zip -o combo
../cpsutil -g jojoba -split -i ./combo/ -o split
../cpsutil -combine -i ../working/jojoba.zip -o jojoba-combo.zip
../cpsutil -split -i ./jojoba-combo.zip -o jojoba-split.zip
touch ./hash.txt
md5sum ./combo/10 >> ./hash.txt
md5sum ./combo/20 >> ./hash.txt
md5sum ./combo/30 >> ./hash.txt
md5sum ./combo/31 >> ./hash.txt
md5sum ./combo/40 >> ./hash.txt
md5sum ./combo/41 >> ./hash.txt
md5sum ./combo/50 >> ./hash.txt
md5sum ./combo/51 >> ./hash.txt

echo "check to see if the hashes in hash.txt and target.txt are the same"