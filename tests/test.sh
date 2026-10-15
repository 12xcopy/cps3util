rm -rf ./combo
rm -rf ./split
rm -rf ./hash.txt
../cpsutil -combine -i ../working/jojoba.zip -o combo
../cpsutil -g jojoba -split -i ./combo/ -o split

touch ./hash.txt
bash ./mkhash.sh
../cpsutil -g jojoba -i ../working/jojoba.zip -combine -decrypt -o test
echo "check to see if the hashes in hash.txt and target.txt are the same"

../cpsutil -combine -i ../working/jojoba.zip -o jojoba-combo.zip
../cpsutil -split -i ./jojoba-combo.zip -o jojoba-split.zip


echo "check to see if the hashes in hash.txt and target.txt are the same"
echo "check to see if the hashes in hash.txt and target.txt are the same"
echo "check to see if the hashes in hash.txt and target.txt are the same"
# do it bro
echo "check to see if the hashes in hash.txt and target.txt are the same"
echo "do it bro"