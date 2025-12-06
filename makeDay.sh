day=$1
mkdir day$day
folder=day$day
cp template.cpp $folder.cpp
mv $folder.cpp $folder

mkdir $folder/inputs
echo ""> $folder/inputs.part1_small.txt
echo ""> $folder/inputs.part1.txt

