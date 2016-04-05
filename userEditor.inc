<?php
include_once 'databaseHandler.inc';
include_once 'userFactory.inc';

class UserEditor{

	function processGetRequest($user){	
		//var_dump($user);	
		$user = UserFactory::createFullFromQuery($user->userName);
		self::displayEditForm($null,$user);
	}	
	
	function processPostRequest($user){
			$passWord=$_POST['passWord'];
				
			$errMsg = self::validateEditForm($passWord);
			if(null == $errMsg){				
				//Create a new user object
				$updatedUser   = UserFactory::createFromPost($_POST);	
				$updatedUser->userName = $user->userName;
				$updatedUser->passWord = $user->passWord;
							
				//Store user in DB
				$un = strtoupper($updatedUser->userName);
				
				//optional image upload perhaps
				if(!isset($_POST['delImgCheckBox'])){
					$errMsg = self::validateUserImage($un);
				
					if(null == $errMsg){
						//also give this new user his/her own directory on the server
						$userDir = getcwd() . "/usrs/" . $un;
					
						if(!is_dir($userDir)){
							if(mkdir($userDir , 0777)){
								//echo("User home created=" . $userDir);
							}
							else{							
								HtmlUtil::sendHeader('druidia.com',TRUE);
								echo("User home creation failed=" . $userDir);
								self::displayEditForm($errMsg,$updatedUser);
								HtmlUtil::sendFooter();
								exit(0);
							}
						}					
					
						//move the file to the user directory
						move_uploaded_file($_FILES["file"]["tmp_name"], $userDir . "/" . $_FILES["file"]["name"]);
						//append the file name path
						$updatedUser->userImagePath = "/usrs/" . $un . "/" . $_FILES["file"]["name"];
					}		
					else{//ERROR WITH FILE UPLOAD VALIDATION
						HtmlUtil::sendHeader('druidia.com',TRUE);
						self::displayEditForm($errMsg,$updatedUser);
						HtmlUtil::sendFooter();
						exit(0);
					}
				}
				else{
					$updatedUser->userImagePath=null;
				}									
				
				$query = "update user set PASSWORD='$updatedUser->passWord',LAST_NAME='$updatedUser->lastName',
				          FIRST_NAME='$updatedUser->firstName',EMAIL='$updatedUser->email',
				          USER_IMAGE_PATH='$updatedUser->userImagePath' where USER_NAME='$updatedUser->userName'";
				          			
				DataBaseHandler::query($query);
				//Store user in session
				SessionHandler::getInstance()->$user = $updatedUser;//store away user in session								
				
				//re-direct to user home page				
				HtmlUtil::redirect('home.php');
				exit(0);
			}
			else{
				HtmlUtil::sendHeader('druidia.com',TRUE);
				self::displayEditForm($errMsg,$updatedUser);
				HtmlUtil::sendFooter();
			}
	}
	
	function validateUserImage($un){
		$allowedExts = array("jpg", "jpeg", "gif", "png");
		$extension   = end(explode(".", $_FILES["file"]["name"]));
						
		//echo "Upload: " . $_FILES["file"]["name"] . "<br />";
		//echo "Type: " . $_FILES["file"]["type"] . "<br />";
		//echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";
		//echo "Size: " . ($_FILES["file"]["size"]);
		//echo "Stored in: " . $_FILES["file"]["tmp_name"];	
				
		//security check
		if ($_FILES["file"]["size"] > 0 && 
			!is_uploaded_file($_FILES["file"]["tmp_name"])) {
			
			return "Possible file upload attack: filename '". $_FILES["file"]["tmp_name"] . "'.";			
		}
				
		if ((($_FILES["file"]["type"] == "image/gif")
		|| ($_FILES["file"]["type"] == "image/jpeg")
		|| ($_FILES["file"]["type"] == "image/pjpeg"))
		&& ( ($_FILES["file"]["size"] / 1024) < 5000)
		&& in_array($extension, $allowedExts))
		  {
		  
		  if ($_FILES["file"]["error"] > 0)
			{
				return "Return Code: " . $_FILES["file"]["error"] . "<br />";
			}
		  else
			{
			//echo "Upload: " . $_FILES["file"]["name"] . "<br />";
			//echo "Type: " . $_FILES["file"]["type"] . "<br />";
			//echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";
			//echo "Temp file: " . $_FILES["file"]["tmp_name"] . "<br />";

			if (file_exists("usrs/" . $un . "/" . $_FILES["file"]["name"]))
			  {
				return $_FILES["file"]["name"] . " already exists.";
			  }
			else
			  {
				return null;
			  }
			}
		  }
		else
		{
		  //not choosing a file is not an error
		  if($_FILES["file"]["size"] == 0){
			  return null;;
		  }
		  else{
			return "Invalid file";
		  }		  
		}		
	}
	
	function validateEditForm($pw){
		$passWord = trim($pw);		
	
		if(!isSet($passWord) ||
		        empty($passWord)){
			return "Invalid Password";			
		}
		
		return null;
	}
	
	function displayEditForm($errMsg,$user){?>
		<table>
			<tr>				
				<td><img src="imgs/druidialogo.png"/></td>
			</tr>
		</table>
		
		<div id="editBox">
			<form name="editForm" action="<?php echo $_SERVER['PHP_SELF'] ?>" method="POST" enctype="multipart/form-data">	
			<center><label>Account Details</label>
			<?php
			if(!empty($errMsg)){
				echo("<br><b><font color=\"red\">" . $errMsg . "</font></b>");				
			}
			?>
			<table>
			<tr>
			<td>
				<table>
					<tr>
						<td><label for='passwd'>Password</label></td>
						<td><input type="password" name="passWord" maxlength='64'/></td>
					</tr>
					<tr>
						<td><label>Last Name</label></td>
						<td><input type="text" name="lastName" maxlength='64' value="<?= $user->lastName ?>"/></td>
					</tr>	
					<tr>
						<td><label>First Name</label></td>
						<td><input type="text" name="firstName" maxlength='64' value="<?= $user->firstName ?>"/></td>
					</tr>	
					<tr>
						<td><label>Email</label></td>
						<td><input type="text" name="email" length="150" maxlength='90' value="<?= $user->email ?>"/></td>
					</tr>							
					<tr>
						<td>
						<input type="submit" name="editSubmit" value="Submit"/>	
						<a href="http://www.druidia.com/home.php">Cancel</a>					
						</td>
					</tr>
				</table>
			</td>
			<td>			
				<img src="<?=$user->userImagePath ?>" height=\"50\" width=\"50\"/>
				<label for='checkbox'>Del</label>
				<input type="checkbox" name="delImgCheckBox"/>
				<br>
				<label>Image(5mb max):</label><br>
			    <input type="file" name="file" id="file"/>			
			<td>
			</tr>
			
			</table>
			</center>
			</form>
		</div>												
		<script type="text/javascript" language="JavaScript">
			document.forms['editForm'].elements['lastName'].focus();
		</script><?php
	}
	
}