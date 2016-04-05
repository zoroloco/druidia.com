<?php
include_once 'user.inc';
include_once 'sessionHandler.inc';
include_once 'databaseHandler.inc';
include_once 'htmlUtil.inc';
//include_once 'board.inc';

run();

	function run(){
		if($_SERVER['REQUEST_METHOD']=='GET'){
			processGetRequest();
		}else if($_SERVER['REQUEST_METHOD']=='POST'){
			processPostRequest();
		}else{
			die("This script only works with GET and POST requests.");
		}
	}
		
	function processGetRequest(){
		HtmlUtil::sendHeader('druidia.com',TRUE);
		//$userName = $_GET['userName'];
		displayRegisterForm(null,null);
		//Board::display(null);
		HtmlUtil::sendFooter();
	}
		
	function processPostRequest(){
		if(isSet($_POST['userName']) || isSet($_POST['passWord'])){
			setcookie ("cookieUserName", "", time() - 3600);//previous user may have had log-in cookie set, so delete old cookie			
			
			$userName=$_POST['userName'];
			$passWord=$_POST['passWord'];
				
			$errMsg = validateRegistrationForm($userName,$passWord);
			if(null == $errMsg){				
				//Create a new user object
				$newUser   = UserFactory::createFromPost($_POST);				
				//Store user in DB
				$un = strtoupper($newUser->userName);
				
				//optional image upload perhaps
				$errMsg = validateUserImage($un);
				
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
							displayRegisterForm($errMsg,$userName);
							//Board::display(null);
							HtmlUtil::sendFooter();
							exit(0);
						}
					}					
					
					//move the file to the user directory
					move_uploaded_file($_FILES["file"]["tmp_name"], $userDir . "/" . $_FILES["file"]["name"]);
					//append the file name path
					$newUser->userImagePath = "/usrs/" . $un . "/" . $_FILES["file"]["name"];
				}		
				else{//ERROR WITH FILE UPLOAD VALIDATION
					HtmlUtil::sendHeader('druidia.com',TRUE);
					displayRegisterForm($errMsg,$userName);
					//Board::display(null);
					HtmlUtil::sendFooter();
					exit(0);
				}								
				
				//priv defaults to 4, which is for guest
				$query = "insert into user (USER_NAME,PASSWORD,USER_PRIV,LAST_NAME,FIRST_NAME,SEX,EMAIL,USER_IMAGE_PATH) 
					values('$un','$newUser->passWord','4','$newUser->lastName',
						   '$newUser->firstName','$newUser->sex','$newUser->email','$newUser->userImagePath')";
					
				DataBaseHandler::query($query);
				//Store user in session
				SessionHandler::getInstance()->$user = $newUser;//store away user in session								
				
				//re-direct to user home page				
				HtmlUtil::redirect('home.php');
				exit(0);
			}
			else{
				HtmlUtil::sendHeader('druidia.com',TRUE);
				displayRegisterForm($errMsg,$userName);
				//Board::display(null);
				HtmlUtil::sendFooter();
			}
		}
		/*
		else if(isSet($_POST['boardTextAreaName'])){						
			HtmlUtil::sendHeader('druidia.com',TRUE);						
			displayRegisterForm(null,null);						
			Board::processPostRequest(null);
			HtmlUtil::sendFooter();
		}
		*/
	}
		
	function validateRegistrationForm($un,$pw){
		$userName = trim($un);
		$passWord = trim($pw);		
	
		if(!isSet($userName) ||
		    empty($userName)){
			return "Invalid Username";									
		}
		else if(!isSet($passWord) ||
		        empty($passWord)){
			return "Invalid Password";			
		}
		else if(isDuplicateUserName($userName)){
			return "Username not available";
		}					
		
		return null;
	}
		
	function isDuplicateUserName($userName){
		$query    = "select * from user u where u.USER_NAME =\"".strtoupper($userName)."\"";
		$result   = DataBaseHandler::query($query);
		$num_rows = mysql_num_rows($result);
	
		if($num_rows>0)
			return true;
	
		return false;
	}
		
	function displayRegisterForm($errMsg,$un){?>
		<table>
			<tr>				
				<td><img src="imgs/druidialogo.png"/></td>
			</tr>
		</table>
		
		<div id="RegistrationBox">
			<form name="registerForm" action="<?php echo $_SERVER['PHP_SELF'] ?>" method="POST" enctype="multipart/form-data">	
			<center><label>Register</label>
			<?php
			if(!empty($errMsg)){
				echo("<br><b><font color=\"red\">" . $errMsg . "</font></b>");				
			}
			?>
			<table>
			<tr>				
				<td><label>Username*</label></td>
				<td><input type="text" name="userName" maxlength='90' value="<?= $un ?>"/></td>
			</tr>					
			<tr>
				<td><label for='passwd'>Password*</label></td>
				<td><input type="password" name="passWord" maxlength='64'/></td>
			</tr>
			<tr>
				<td><label>Last Name</label></td>
				<td><input type="text" name="lastName" maxlength='64'/></td>
			</tr>	
			<tr>
				<td><label>First Name</label></td>
				<td><input type="text" name="firstName" maxlength='64'/></td>
			</tr>	
			<tr>
				<td><input type="radio" name="sex" value="m" />Male</td>
				<td><input type="radio" name="sex" value="f" />Female</td>
			</tr>
			<tr>
				<td><label>Email</label></td>
				<td><input type="text" name="email" length="150" maxlength='90' value="<?= $fn ?>"/></td>
			</tr>
			
			<tr>
				<td><label>Image(5mb max):</label></td>
			    <td><input type="file" name="file" id="file"/></td>
			</tr>
			<tr>
				<td>
				<input type="submit" value="Register"/>	
				<a href="http://www.druidia.com/index.php">Cancel</a>					
				</td>
			</tr>
			
			</table>
			</center>
			</form>
		</div>												
		<script type="text/javascript" language="JavaScript">
			document.forms['registerForm'].elements['userName'].focus();
		</script><?php
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
	
	?>
			
	</body>
</html>