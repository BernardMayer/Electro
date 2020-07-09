CREATE multiSET TABLE DMUP10DTWH_BKP_0.US04_bo ,NO FALLBACK ,
	NO BEFORE JOURNAL,
	NO AFTER JOURNAL,
	CHECKSUM = DEFAULT,
	DEFAULT MERGEBLOCKRATIO
	(
	CD_EDS_BNQ INTEGER NOT NULL,
	NUMPART SMALLINT,
	DTSIT TIMESTAMP(6),
	TYPSIT CHAR(1) CHARACTER SET LATIN NOT CASESPECIFIC,

	IDCMVT INTEGER,

	IDTERM	CHAR(4),
	DASTDQ	TIMESTAMP(6),
	HEURE5	TIMESTAMP(6),
	/* NUSEQ7	CHAR(1), */
	CDOPGA	CHAR(2),
	CDFCT1	CHAR(1),
	CDRETG	CHAR(3),
	CBINAA	CHAR(1),
	ZB004B	CHAR(4),
	NUACG0	CHAR(4),
	DATE6N	TIMESTAMP(6),
	HEURE2	TIMESTAMP(6),
	NOOPG	CHAR(6),
	NOARGC	CHAR(5),
	CBIND3	CHAR(1),
	CBIND8	CHAR(1),
	CDFMT1	CHAR(1),
	/* ZB001	CHAR(1), */
	
	NUSEQ7 CHAR(1) CHARACTER SET LATIN NOT CASESPECIFIC,
	CDRET1 CHAR(2) CHARACTER SET LATIN NOT CASESPECIFIC,
	CDRET2 CHAR(2) CHARACTER SET LATIN NOT CASESPECIFIC,
	CDRETX CHAR(2) CHARACTER SET LATIN NOT CASESPECIFIC,
	IDSUC2 CHAR(19) CHARACTER SET LATIN NOT CASESPECIFIC,
	ZB003 CHAR(3) CHARACTER SET LATIN NOT CASESPECIFIC,
	CDEVIS CHAR(3) CHARACTER SET LATIN NOT CASESPECIFIC,
	MTGABA DECIMAL(15,2),
	NUAUTO CHAR(6) CHARACTER SET LATIN NOT CASESPECIFIC,
	CDMLP CHAR(2) CHARACTER SET LATIN NOT CASESPECIFIC,
	MTGABD DECIMAL(15,2),
	ZB006 CHAR(6) CHARACTER SET LATIN NOT CASESPECIFIC,
	MTGAB DECIMAL(15,2),
	ZB002B CHAR(2) CHARACTER SET LATIN NOT CASESPECIFIC,
	MTGABC DECIMAL(15,2),
	MTGABH DECIMAL(15,2),
	MTGABF DECIMAL(15,2),
	ZB001 CHAR(1) CHARACTER SET LATIN NOT CASESPECIFIC,
	NOSEMG INTEGER,
	DAA CHAR(2) CHARACTER SET LATIN NOT CASESPECIFIC,
	DEXPC TIMESTAMP(6),
	DMM INTEGER,
	NBBIL1 INTEGER,
	NBBIL2 INTEGER,
	NBBIL3 INTEGER,
	NBBIL INTEGER,
	IDRES3 CHAR(3) CHARACTER SET LATIN NOT CASESPECIFIC,
	CDTRTI CHAR(1) CHARACTER SET LATIN NOT CASESPECIFIC,
	CDAUT CHAR(1) CHARACTER SET LATIN NOT CASESPECIFIC,
	NUTRAC CHAR(15) CHARACTER SET LATIN NOT CASESPECIFIC,
	NUAUT1 CHAR(4) CHARACTER SET LATIN NOT CASESPECIFIC,
	DOOPFI TIMESTAMP(6),
	NCPTEX CHAR(11) CHARACTER SET LATIN NOT CASESPECIFIC,
	ZB002 CHAR(2) CHARACTER SET LATIN NOT CASESPECIFIC,
	NUBIL4 CHAR(6) CHARACTER SET LATIN NOT CASESPECIFIC,
	NUBIL5 CHAR(6) CHARACTER SET LATIN NOT CASESPECIFIC,
	NUBIL6 CHAR(6) CHARACTER SET LATIN NOT CASESPECIFIC,
	NUBIL7 CHAR(6) CHARACTER SET LATIN NOT CASESPECIFIC,
	CDPRE CHAR(1) CHARACTER SET LATIN NOT CASESPECIFIC,
	DMENF8 TIMESTAMP(6),
	HOPLSB TIMESTAMP(6),
	DCPTA1 TIMESTAMP(6),
	DTARR TIMESTAMP(6),
	CDMODI CHAR(1) CHARACTER SET LATIN NOT CASESPECIFIC,
	IDSUSP CHAR(1) CHARACTER SET LATIN NOT CASESPECIFIC
)
PRIMARY INDEX ( CD_EDS_BNQ ,IDTERM ,CDFCT1 ,NUAUTO ,NUTRAC ,NCPTEX ,HOPLSB )
