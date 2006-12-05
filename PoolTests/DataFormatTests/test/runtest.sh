mkdir -p $CMSSW_BASE/src/testDF/testDF/
cp -r $CMSSW_BASE/src/PoolTests/DataFormatTests/test $CMSSW_BASE/src/testDF/testDF/.
cd $CMSSW_BASE/src/testDF/testDF/test
python  generateTests.py  $CMSSW_RELEASE_BASE/src
mv BuildFile_genH BuildFile
scramv1 b -k -j 4
touch write.log
foreach x ( $CMSSW_BASE/test/$SCRAM_ARCH/edm* )
$x >>& write.log
end
grep "Warning" write.log
grep "Fatal" write.log
grep "what()" write.log
touch read.log
foreach x ( *.root )
$CMSSW_BASE/test/$SCRAM_ARCH/test_autoload_pooltree $x >>& read.log
end
