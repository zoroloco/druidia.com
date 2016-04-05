<?php
include_once 'databaseHandler.inc';
include_once 'sessionHandler.inc';
include_once 'privilegeHandler.inc';

class Notes{

	function processGetRequest($user){	
		$delKey    = $_GET['del'];
		$editKey   = $_GET['editNote'];
				
		if(null != $delKey && isSet($delKey)){
			echo("<div id=\"NotesBoard\">");					
			self::displayForm(null,$user);
			self::displaySighs($delKey,$user,null);
			echo("</div>");
		}
		else if(null != $editKey && isSet($editKey)){
			echo("<div id=\"NotesBoard\">");					
			self::displayForm(null,$user);
			self::displaySighs(null,$user,$editKey);
			echo("</div>");
		}
		else{
			self::display($user);
		}
	}

	function processPostRequest($user){	
		//only process post if it is for board form
		if(!empty($_POST['notesBoardSubmit'])){
			//set messsage text
			$text   = strip_tags($_POST['notesBoardTextAreaName']);
		
			if(self::validateSigh()){
				//set username
				$userName = null;			
				if(null != $user && isSet($user)){
					$userName = $user->userName;
				}
			
				//set date
				$date = date('F jS Y h:i:s A');
			
				//do insert
				$query = "insert into notes (USER,MESSAGE,DATE) values('$user->userName','$text','$date')";	
				DataBaseHandler::query($query);
			
				self::display($user);
			}
			else{					
				echo("<div id=\"NotesBoard\">");					
				self::displayForm($text,$user);
				self::displaySighs(null,$user,null);
				echo("</div>");
			}
		}
		else if(!empty($_POST['editNotesBoardSubmit'])){
			$text = strip_tags($_POST['editNotesBoardTextAreaName']);
			$rowId= $_POST['rowId'];
			
			$userName = null;			
			if(null != $user && isSet($user)){
				$userName = $user->userName;
			}
			
			//set date
			$date = date('F jS Y h:i:s A');
			
			//do insert
			$query = "update notes n set n.MESSAGE = '$text', n.DATE='$date' where n.USER='$user->userName' and n.ID='$rowId'";
						
			DataBaseHandler::query($query);
			
			self::display($user);
		}
		else{//just show board					
			echo("<div id=\"NotesBoard\">");					
			self::displayForm($text,$user);
			self::displaySighs(null,$user,null);
			echo("</div>");
		}
	}
	
	function validateSigh(){
		$errMsg = null;
		
		if(isSet($_POST['notesBoardTextAreaName'])){
			$text = strip_tags($_POST['notesBoardTextAreaName']);
			if(null != $text){
				if(strlen(trim($text))>0){
					if(strlen(trim($text))<5000){
						return true;
					}
					else{
						$errMsg = "<font color=\"red\">Your notes must be less than 5000 characters! Don't write me your life story!</font>";
					}
				}
				else{
					$errMsg = "<font color=\"red\">Can you please enter your notes?</font>";
				}
			}
			else{
				$errMsg = "<font color=\"red\">Can you please enter your notes?</font>";
			}
		}
		else{
			$errMsg = "<font color=\"red\">Can you please enter your notes?</font>";
		}
		
		echo($errMsg);
	}

	function displayForm($sighText,$user){
	?>
		<div id="NotesForm">
		<form name="boardForm" action="<?php echo $_SERVER['PHP_SELF'] ?>" method="POST">	
		Leave a private note<br>
		
		<?php
			UserFactory::storeUserInPost($user);
		?>
		
		<textarea name="notesBoardTextAreaName" rows="15" cols="55"></textarea>
		<input type="submit" name="notesBoardSubmit" value="Submit"/>	
		</form>
	</div>
	<?php			
	}
	
	function deleteRow($rowNum){
		$query ="DELETE FROM notes WHERE ID='$rowNum'";
		DataBaseHandler::query($query);
	}
	
	function displaySighs($rowToDelete,$user,$rowToEdit){
		$query = "SELECT * FROM notes n where n.USER = \"".strtoupper($user->userName)."\"";
		 
		 $result   = DataBaseHandler::query($query);		   		  		   
		 $num_rows = mysql_num_rows($result);
		 
		 $privHandler = new PrivilegeHandler($user);
		 $count = 0;
		 if($num_rows){
			echo("<div id=\"Notes\">");
		 
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
					echo("<br><div id=\"EvenNotes\">");				
				}
				else{
					echo("<br><div id=\"OddNotes\">");				
				}
				
				//message contents
				
				//EDIT-MODE
				if(null != $rowToEdit && 0 == strcmp($rowToEdit,$row['ID'])){	
					?>				   
					<form name="editNoteForm" action="<?php echo $_SERVER['PHP_SELF'] ?>" method="POST">
					<?php UserFactory::storeUserInPost($user); 
					      echo("<input type=\"hidden\" name=\"rowId\" value=\"" . $rowToEdit . "\"/>");
					?>					
					<textarea name="editNotesBoardTextAreaName" rows="15" cols="55"><?php echo($row['MESSAGE']);?></textarea>
					<input type="submit" name="editNotesBoardSubmit" value="Update"/>	
					</form>				  				   
					<?php
				}
				//READ-ONLY
				else{															
					echo("<table><tr>");
					echo("<td>" . $row['MESSAGE']."<br>");																
					//signature
					echo("<font size=\"1\" color=\"blue\">");											
					echo("Last updated ".$row['DATE']. "     ");
				
					if(null != $user && 
						isSet($user) && 
						(0 == strcasecmp($userName,$user->userName) || $privHandler->hasPrivilege(PrivilegeHandler::PRIV_ADMIN))){
						echo("<a href=\"http://www.druidia.com/home.php?editNote=");
						echo $row['ID'];					
						echo("\"><img src=\"imgs/editicon.png\" height=\"20\" width=\"20\"/></a>");
				   
						echo("<a href=\"http://www.druidia.com/home.php?del=");
						echo $row['ID'];					
						echo("\"><img src=\"imgs/trashicon.png\" height=\"20\" width=\"20\"/></a>");										
					}							
					echo("</font></td></tr></table>");
				
					echo("</div>");				
					$count++;					
				}
			}//loop of all notes
			
			echo("</div>");
		 }
	}
	
	/*
	 * called externally
	 *
	*/
	function display($user){	
		echo("<div id=\"NotesBoard\">");	
		self::displayForm(null,$user,null);
		self::displaySighs(null,$user,null);
		echo("</div>");
	}
}
?>