<?php
include_once 'databaseHandler.inc';
include_once 'sessionHandler.inc';
include_once 'privilegeHandler.inc';

class Board{

	function processGetRequest($user){	
		$delKey = $_GET['del'];
				
		if(null != $delKey && isSet($delKey)){
			echo("<div id=\"Board\">");					
			self::displayForm(null,$user);
			self::displaySighs($delKey,$user);
			echo("</div>");
		}
		else{
			self::display($user);
		}
	}

	function processPostRequest($user){	
		//only process post if it is for board form
		if(!empty($_POST['boardSubmit'])){
			//set messsage text
			$text   = strip_tags($_POST['boardTextAreaName']);
		
			if(self::validateSigh()){
				//set username
				$userName = null;			
				if(null != $user && isSet($user)){
					$userName = $user->userName;
				}
			
				//set date
				$date = date('F jS Y h:i:s A');
			
				//do insert
				$query = "insert into board (USER,MESSAGE,DATE) values('$user->userName','$text','$date')";	
				DataBaseHandler::query($query);
			
				self::display($user);
			}
			else{					
				echo("<div id=\"Board\">");					
				self::displayForm($text,$user);
				self::displaySighs(null,$user);
				echo("</div>");
			}
		}
		else{//just show board					
			echo("<div id=\"Board\">");					
			self::displayForm($text,$user);
			self::displaySighs(null,$user);
			echo("</div>");
		}
	}
	
	function validateSigh(){
		$errMsg = null;
		
		if(isSet($_POST['boardTextAreaName'])){
			$text = strip_tags($_POST['boardTextAreaName']);
			if(null != $text){
				if(strlen(trim($text))>0){
					if(strlen(trim($text))<1000){
						return true;
					}
					else{
						$errMsg = "<font color=\"red\">Your message must be less than 1000 characters! Don't write me your life story!</font>";
					}
				}
				else{
					$errMsg = "<font color=\"red\">Can you please enter your message?</font>";
				}
			}
			else{
				$errMsg = "<font color=\"red\">Can you please enter your message?</font>";
			}
		}
		else{
			$errMsg = "<font color=\"red\">Can you please enter your message?</font>";
		}
		
		echo($errMsg);
	}

	function displayForm($sighText,$user){
	?>
		<div id="SighForm">
		<form name="boardForm" action="<?php echo $_SERVER['PHP_SELF'] ?>" method="POST">	
		Leave a public message<br>
		
		<?php
			UserFactory::storeUserInPost($user);
		?>
		
		<textarea autofocus name="boardTextAreaName" rows="4" cols="45"></textarea>
		<input type="submit" name="boardSubmit" value="Submit"/>	
		</form>
	</div>
	<?php			
	}
	
	function deleteRow($rowNum){
		$query ="DELETE FROM board WHERE ID='$rowNum'";
		DataBaseHandler::query($query);
	}
	
	function displaySighs($rowToDelete,$user){
		 //$query = "select * from board order by ID desc";
		$query = "SELECT b.*,u.USER_IMAGE_PATH FROM board b
                  LEFT JOIN user u ON b.USER = u.USER_NAME";
		 
		 $result   = DataBaseHandler::query($query);		   		  		   
		 $num_rows = mysql_num_rows($result);
		 
		 $privHandler = new PrivilegeHandler($user);
		 $count = 0;
		 if($num_rows){
			echo("<div id=\"Sighs\">");
		 
			while($row = mysql_fetch_array($result))
			{	
				//determine user for this current row
				$userName = $row['USER'];
				if(null == $userName || !isSet($userName))
					$userName = 'Anonymous';
						
				//delete row from DB and don't display
				if(null != $user &&
				   isSet($user) &&
				   (0 == strcasecmp($userName,$user->userName) || $privHandler->hasPrivilege(PrivilegeHandler::PRIV_ADMIN)) &&
				   null != $rowToDelete &&
				   0 == strcmp($rowToDelete,$row['ID'])){				   
				   
				   self::deleteRow($rowToDelete);
				   continue;
				}
			
				if($count%2==0){
					echo("<br><div id=\"EvenSighs\">");				
				}
				else{
					echo("<br><div id=\"OddSighs\">");				
				}
				
				//message contents
				echo("<table><tr>");
				
				if(isset($row['USER_IMAGE_PATH']) &&
				   !empty($row['USER_IMAGE_PATH'])){
					echo("<td><img src=\"" . $row['USER_IMAGE_PATH'] . "\" height=\"75\" width=\"75\"/></td>");
				}
				
				echo("<td>" . $row['MESSAGE']."<br>");												
				
				//signature
				echo("<font size=\"1\" color=\"blue\">");											
				echo("Posted by ".$userName." on ".$row['DATE']);
				
				if(null != $user && 
				   isSet($user) && 
				   (0 == strcasecmp($userName,$user->userName) || $privHandler->hasPrivilege(PrivilegeHandler::PRIV_ADMIN))){
					echo("<p style=\"text-align:right;\"><a href=\"http://www.druidia.com/home.php?del=");
					echo $row['ID'];
					echo("\"><img src=\"imgs/trashicon.png\" height=\"20\" width=\"20\"/></a></p>");
				}			
				
				echo("</font></td></tr></table>");
				
				echo("</div>");
				
				$count++;
			}
			
			echo("</div>");
		 }
	}
	
	/*
	 * called externally
	 *
	*/
	function display($user){	
		echo("<div id=\"Board\">");	
		self::displayForm(null,$user);
		self::displaySighs(null,$user);
		echo("</div>");
	}
}
?>