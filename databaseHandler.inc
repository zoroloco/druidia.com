<?php

class DatabaseHandler{

	/**
	 * Static	 
	*/
	function query($query){
		$dbName     = "ENTER YOUR DATABASE NAME HERE";
		$dbUserName = "YOUR DATABASE USER NAME";
		$dbPw       = "YOUR DATABASE PASSWORD";
		$dbURL      = "YOUR DATABASSE URL";
	
		mysql_connect($dbURL,$dbUserName,$dbPw);
		@mysql_select_db($dbName) or die( "Unable to select database: ".$dbName);								
		$result = mysql_query($query);				
		
		mysql_close();
		
		return $result;
	}
}

?>