
echo "WARNING!!!!! CHECK #GENERATED!!!!! SHOULD BE SAME AS IN GAMOS 'in' FILE"
read okcontinue

dirs=$( ls -1 | /bin/grep E6 )

ACTIVS=${dirs}

numGen=100000

if [ "$numGen" != "$okcontinue" ]; then
    echo WRONG NUMBER OF \#GENERATED: $numGen != $okcontinue 
	return
fi

#	echo DIRS: ${ACTIVS}

procfilename=proc_DataTT.out
#	rejectedfname=rejected.dat
#	rejectedfname=rejected_100.dat
rejectedfname=accepted_R101.dat

#	echo Analysing files: $promptsfilename and $procfilename
#	echo Analysing file: $procfilename

for actDir in $ACTIVS
do 
	activity=$( echo $actDir | cut -f7 -d_ )

	# echo dir: ${actDir} with activity: ${activity}
	
	DIR=$actDir
	#	numAll=$( wc $DIR/${promptsfilename} | awk '{ print $1 }' )
	numAll=$( grep "NC_prompt " $DIR/${procfilename} | awk '{ print $4 }' )
	
	numGood=$( grep "NC_good " $DIR/${procfilename} | grep -v "LOR" | awk '{ print $4 }' )
	
	numRandom=$( grep "NC_rand " $DIR/${procfilename} | awk '{ print $4 }' )
	numScat=$( grep "NC_scat " $DIR/${procfilename} | awk '{ print $4 }' )

	numBad=$( echo $numRandom + $numScat | bc -l )
	
	
	echo  "activity  #Gen     #All    #True   #Random    #Scat"
	echo  $activity "  " $numGen "  " $numAll "  " $numGood "  " $numRandom "  " $numScat
	#	echo " ============================== "

    nrejectedR=$( grep "randoms" $DIR/${rejectedfname} | awk '{ print $4 }' )
    nrejectedS=$( grep "scattered" $DIR/${rejectedfname} | awk '{ print $4 }' )
    nrejectedT=$( grep "trues" $DIR/${rejectedfname} | awk '{ print $4 }' )

	echo "# rejected Random, Scattered, True: " $nrejectedR  " "  $nrejectedS  " " $nrejectedT

    numAll=$( echo $numAll - $nrejectedR - $nrejectedS - $nrejectedT| bc -l )
    numRandom=$( echo $numRandom - $nrejectedR | bc -l )
    numScat=$( echo $numScat - $nrejectedS | bc -l )
    numGood=$( echo $numGood - $nrejectedT | bc -l )
    echo "Corrected numbers: "
    echo  $activity "  " $numGen "  " $numAll "  " $numGood "  " $numRandom "  " $numScat
	echo " "

done

