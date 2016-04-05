CREATE TABLE `board` (
  `ID` bigint(20) NOT NULL auto_increment,
  `USER` varchar(80) default 'Anonymous',
  `MESSAGE` varchar(1024) NOT NULL,
  `DATE` varchar(250) NOT NULL,
  `PERSON` varchar(80) NOT NULL,
  `SIGH` varchar(80) NOT NULL,
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=224 ;

CREATE TABLE `notes` (
  `ID` bigint(20) NOT NULL auto_increment,
  `USER` varchar(80) default 'Anonymous',
  `MESSAGE` varchar(1024) NOT NULL,
  `DATE` varchar(250) NOT NULL,
  `PERSON` varchar(80) NOT NULL,
  `SIGH` varchar(80) NOT NULL,
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=228 ;

CREATE TABLE `privileges` (
  `NAME` varchar(20) NOT NULL,
  `VALUE` bigint(20) NOT NULL,
  PRIMARY KEY  (`NAME`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `privileges` VALUES('ADMIN', 1);
INSERT INTO `privileges` VALUES('GLOBAL_VIEW', 2);
INSERT INTO `privileges` VALUES('GUEST', 4);
INSERT INTO `privileges` VALUES('WHATEVER', 8);

CREATE TABLE `user` (
  `USER_NAME` varchar(96) NOT NULL,
  `PASSWORD` varchar(64) NOT NULL,
  `LAST_NAME` varchar(64) default NULL,
  `FIRST_NAME` varchar(64) default NULL,
  `BIRTH_DATE` date default NULL,
  `EMAIL` varchar(64) default NULL,
  `PROFILE_ID` int(11) NOT NULL auto_increment,
  `SEX` varchar(1) default NULL,
  `USER_IMAGE_PATH` varchar(120) default NULL,
  `LAST_LOGIN_DATE` date default NULL,
  `USER_PRIV` bigint(20) NOT NULL,
  `SESSION_VAR` varchar(100) NOT NULL,
  PRIMARY KEY  (`USER_NAME`),
  UNIQUE KEY `PROFILE_ID` (`PROFILE_ID`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=26 ;

