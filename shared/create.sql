create table `FES` (
`id` int NOT NULL ,
`description` varchar(50) NOT NULL ,
`feldid` varchar(50) NOT NULL 
) TYPE = innodb;

create table `BOOKS` (
`id` int NOT NULL ,
`type` enum( 'BOOK','CD','DVD','VHS') NOT NULL default 'BOOK' ,
`title` varchar(255) NOT NULL ,
`author` varchar(100) NOT NULL ,
`description` text NOT NULL ,
`isbn` varchar(50) NOT NULL ,
`identifier` varchar(50) NOT NULL ,
`location` varchar(50) NOT NULL ,
`owner` varchar(50) NOT NULL ,
`comment` varchar(50) NOT NULL 
) TYPE = innodb;

create table `NOTES_HEADS` (
`id` int NOT NULL ,
`title` varchar(50) NOT NULL ,
`hist_anzeit` int NOT NULL ,
`hist_aezeit` int NOT NULL ,
`status` enum( 'OPEN','PENDING','CLOSED') NOT NULL default 'OPEN' 
) TYPE = innodb;

create table `NOTES_LINES` (
`id` int NOT NULL ,
`head_id` int NOT NULL ,
`hist_anzeit` int NOT NULL ,
`hist_aezeit` int NOT NULL ,
`note` text NOT NULL ,
`type` enum( 'NOTE','ALERT') NOT NULL ,
`status` enum( 'OPEN','PENDING','CLOSED') NOT NULL default 'OPEN' 
) TYPE = innodb;

create table `SEQUENCE` (
`name` varchar(25) NOT NULL ,
`from` int NOT NULL ,
`to` int NOT NULL ,
`nextval` int NOT NULL ,
`format` varchar(50) NOT NULL 
) TYPE = innodb;

ALTER TABLE `FES` ADD PRIMARY KEY ( `id` );
ALTER TABLE `FES` CHANGE `id` `id` int NOT NULL auto_increment ;
ALTER TABLE `FES` CHANGE `feldid` `feldid` varchar(50) NOT NULL unique ;

ALTER TABLE `BOOKS` ADD PRIMARY KEY ( `id` );
ALTER TABLE `BOOKS` CHANGE `id` `id` int NOT NULL auto_increment ;

ALTER TABLE `NOTES_HEADS` ADD PRIMARY KEY ( `id` );
ALTER TABLE `NOTES_HEADS` CHANGE `id` `id` int NOT NULL auto_increment ;

ALTER TABLE `NOTES_LINES` ADD PRIMARY KEY ( `id` );
ALTER TABLE `NOTES_LINES` CHANGE `id` `id` int NOT NULL auto_increment ;
ALTER TABLE `NOTES_LINES` ADD INDEX ( `head_id`);

ALTER TABLE `SEQUENCE` ADD PRIMARY KEY ( `name` );
ALTER TABLE `SEQUENCE` CHANGE `name` `name` varchar(25) NOT NULL unique ;

ALTER TABLE `NOTES_LINES` ADD FOREIGN KEY ( `head_id`) REFERENCES `NOTES_HEADS` ( `id` ) ON DELETE RESTRICT;
