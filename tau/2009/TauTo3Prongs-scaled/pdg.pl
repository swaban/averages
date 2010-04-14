#!/usr/local/bin/perl

# print $ARGV[0]."\n";

$rescale = 1.211572;
$sfac1 = 1.61983 * $rescale;
$sfac2 = 2.40957 * $rescale;
$sfac3 = 2.32292 * $rescale;
$sfac4 = 5.43807 * $rescale;
$sfac5 = 1.42995 * $rescale;

printf "* B(tau- -> pi- pi- pi+ nu) [ex K0]\n";
printf "BEGIN       CLEO3 PimPimPipNu published BRIERE   03\n";
printf "\n";
printf "MEASUREMENT m_PimPimPipNu statistical systematic\n";
printf "DATA        m_PimPimPipNu statistical systematic\n";
printf "            9.13e-2       %0.5e %0.5e\n", 0.05e-2 * $sfac1, 0.46e-2 * $sfac1;
printf "\n";
printf "DATA\n";
printf "            Cleo3Other_00 %0.5e\n", 5e-2 * 9.13e-2 * $sfac1;
printf "            Cleo3Other_1u %0.5e\n", sqrt(0.46e-2**2 -  5e-2**2 * 9.13e-2 **2) * $sfac1;
printf "\n";
printf "END\n";
printf "\n";
printf "* * B(tau- -> pi- K- pi+ nu) [ex. K0]\n";
printf "* BEGIN       OPAL PimKmPipNu published  ABBIENDI  04J\n";
printf "* \n";
printf "* MEASUREMENT m_PimKmPipNu statistical systematic\n";
printf "* DATA        m_PimKmPipNu statistical systematic\n";
printf "*             0.415e-2     %0.5e 0.0\n", sqrt ( 0.053e-2 **2 + 0.040e-2 **2 ) * $sfac2;
printf "* \n";
printf "* END\n";
printf "\n";
printf "BEGIN       CLEO3 PimKmPipNu published BRIERE    03\n";
printf "\n";
printf "MEASUREMENT m_PimKmPipNu statistical systematic\n";
printf "DATA        m_PimKmPipNu statistical systematic\n";
printf "            0.384e-2     %0.5e %0.5e\n",0.014e-2 * $sfac2, 0.038e-2 * $sfac2;
printf "\n";
printf "DATA\n"; 
printf "            Cleo3Other_00 %0.5e\n", 5e-2 * 0.384e-2 * $sfac2;
printf "            Cleo3Other_2u %0.5e\n", sqrt ( 0.038e-2 **2 - 5e-2 **2 * 0.384e-2**2 ) * $sfac2;
printf "\n";
printf "END\n";
printf "\n";
printf "* BEGIN       CLEO PimKmPipNu published RICHICHI   99\n";
printf "* \n";
printf "* MEASUREMENT m_PimKmPipNu statistical systematic\n";
printf "* DATA        m_PimKmPipNu statistical systematic\n";
printf "*             0.346e-2     %0.5e 0.0\n", sqrt ( 0.023e-2 **2 + 0.056e-2 **2) * $sfac2;
printf "* \n";
printf "* END\n";
printf "\n";
printf "BEGIN       ALEPH PimKmPipNu published BARATE    98\n";
printf "\n";
printf "MEASUREMENT m_PimKmPipNu statistical systematic\n";
printf "DATA        m_PimKmPipNu statistical systematic\n";
printf "            0.214e-2     %0.5e 0.0\n", sqrt ( 0.037e-2 **2 + 0.029e-2 **2) * $sfac2;
printf "\n";
printf "END\n";
printf "\n";
printf "* B(tau- -> pi- K- K+ nu)\n";
printf "BEGIN       CLEO3 PimKmKpNu published   BRIERE  03\n";
printf "\n";
printf "MEASUREMENT m_PimKmKpNu statistical systematic\n";
printf "DATA        m_PimKmKpNu statistical systematic\n";
printf "            1.550e-3    %0.5e %0.5e\n", 0.060e-3 * $sfac3, 0.090e-3 * $sfac3;
printf "\n";
printf "DATA\n"; 
printf "            Cleo3Other_00 %0.5e\n", 5e-2 * 1.550e-3 * $sfac3;
printf "            Cleo3Other_3u %0.5e\n",sqrt ( 0.090e-3 **2 - 5e-2**2 * 1.550e-3**2 ) * $sfac3;
printf "\n";
printf "END\n";
printf "\n";
printf "* BEGIN       OPAL PimKmKpNu published   ABBIENDI 00D\n";
printf "* \n";
printf "* MEASUREMENT m_PimKmKpNu statistical systematic\n";
printf "* DATA        m_PimKmKpNu statistical systematic\n";
printf "*             0.870e-3     %0.5e 0.0\n", sqrt ( 0.560e-3 **2 + 0.400e-3 **2 ) * $sfac3;
printf "* \n";
printf "* END\n";
printf "\n";
printf "* BEGIN       CLEO PimKmKpNu published   RICHICHI 99\n";
printf "* \n";
printf "* MEASUREMENT m_PimKmKpNu statistical systematic\n";
printf "* DATA        m_PimKmKpNu statistical systematic\n";
printf "*             1.450e-3     %0.5e 0.0\n", sqrt ( 0.130e-3 **2 + 0.280e-3 **2 ) * $sfac3;
printf "* \n";
printf "* END\n";
printf "\n";
printf "* BEGIN       ALEPH PimKmKpNu published   BARATE  98\n";
printf "* \n";
printf "* MEASUREMENT m_PimKmKpNu statistical systematic\n";
printf "* DATA        m_PimKmKpNu statistical systematic\n";
printf "*             1.630e-3    %0.5e 0.0\n", sqrt ( 0.210e-3 **2 + 0.170e-3 **2 ) * $sfac3;
printf "* \n";
printf "* END\n";
