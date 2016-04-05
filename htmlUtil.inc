<?php

class HtmlUtil{

	public static function sendHeader($pageTitle,$startBodyFlag){
		echo("<html><head><title>");
		print($pageTitle);
		echo("</title>");
		echo("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">");
		echo("<meta name=\"description\" content=\"Nothing\">");
		echo("<link type=\"text/css\" rel=\"stylesheet\" href=\"http://www.druidia.com/styles.css\">");		
		echo("</head>");		
		
		if($startBodyFlag)
			self::startBody();
	}
	
	public static function startBody(){
		echo("<body><div id=\"Main\">");
	}
	
	public static function sendFooter(){
		echo("</div></body></html>");
	}

	public static function redirect($url){
		echo "<script>location.href='$url'</script>";
	}
	
	public static function isEmpty($str){
		return (null == $str || !isSet($str) || strlen(trim($str))<=0);
	}
}

?>