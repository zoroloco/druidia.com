<?php
include_once 'sessionHandler.inc';
include_once 'user.inc';
include_once 'htmlUtil.inc';
include_once 'databaseHandler.inc';
include_once 'board.inc';

SessionHandler::getInstance();//instantiate session

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

	function processPostRequest(){
		//two possible forms could have sent submit.
		if(!empty($_POST['loginSubmit'])){
			$userName=$_POST['userName'];
			$passWord=$_POST['passWord'];						
			
			if(validateLoginForm($userName,$passWord)){						
				$cookieValue = "false";
				if(isset($_POST['rememberLoginCheckBox'])){
					//get user we just stored in session
					$userSessionKey = 'user';
					$loggedInUser = SessionHandler::getInstance()->$userSessionKey;		
				
					$cookieValue = "true";
					//tie session ID to user in DB
					$query = "UPDATE user SET SESSION_VAR=\"".session_id()."\" WHERE USER_NAME=\"".$loggedInUser->userName."\"";
					//echo($query);
					DataBaseHandler::query($query);
				}
				
				HtmlUtil::redirect("home.php?cookieFlag=".$cookieValue);
				exit(0);
			}
		}
		if(!empty($_POST['boardSubmit'])){
			$userSessionKey = 'user';
			$loggedInUser = SessionHandler::getInstance()->$userSessionKey;//note: may be null
			HtmlUtil::sendHeader('druidia.com',TRUE);
			displayLogin(false,null,null);
			Board::processPostRequest($loggedInUser);
		}
	}
	
	function processGetRequest(){
		if("true"==$_GET['logOut']){
			$userSessionKey = 'user';
			$oldUser = SessionHandler::getInstance()->$userSessionKey;		
			processLogOut($oldUser->userName);
			HtmlUtil::sendHeader('druidia.com',TRUE);
			displayLogin(false,$oldUser->userName,$oldUser->passWord);
			Board::display(null);
			HtmlUtil::sendFooter();
		}
		else if(!isCookieSet()){
			$userSessionKey = 'user';
			$oldUser = SessionHandler::getInstance()->$userSessionKey;		
		    HtmlUtil::sendHeader('druidia.com',TRUE);
			displayLogin(false,null,null);
			Board::display($oldUser);
			HtmlUtil::sendFooter();
		}		
	}
	
	function processLogOut($userName){
		//kill session
		SessionHandler::getInstance()->destroy();
		//expire the cookie
		setcookie("cookieUserName", $userName, time()-3600, "", "www.druidia.com",FALSE);
	}

	function validateLoginForm($userName,$passWord){
		if(isSet($userName) &&
		   isSet($passWord) &&
		   !empty($userName) &&
		   !empty($passWord)){
		   
		   $query = "select * from user u where u.USER_NAME =\"".strtoupper($userName)."\" and u.PASSWORD = \"".$passWord."\"";

		   $result   = DataBaseHandler::query($query);		   		  		   
		   $num_rows = mysql_num_rows($result);
		
		   if(!$num_rows){								   		
				HtmlUtil::sendHeader('druidia.com',TRUE);
				displayLogin(true,$userName,$passWord);
				Board::display(null);
				HtmlUtil::sendFooter();
				return false;
			}
			else{					
				//create session from user
				SessionHandler::getInstance()->$user = UserFactory::createFromQuery($result);
	            return true;
			}		   		   
		}
		else{			
			HtmlUtil::sendHeader('druidia.com',TRUE);
			displayLogin(true,$userName,$passWord);
			Board::display(null);
			HtmlUtil::sendFooter();
		}
		return false;
	}
	
	function isCookieSet(){
		$cookiedUser = UserFactory::createFromCookie();
		//var_dump($cookiedUser);
		if(isset($cookiedUser)){			
			//STORE AWAY user we retrieved via cookie info to the session
			SessionHandler::getInstance()->$user = $cookiedUser;
			//redirect to home page for user
			HtmlUtil::redirect('home.php');
			exit(0);			
		}
	}
	
	function displayLogin($loginFailed,$userName,$pw){?>
		<div id="LoginBox">
			<form name="loginForm" action="<?php echo $_SERVER['PHP_SELF'] ?>" method="POST">	
			
			<?php
			if($loginFailed){
				print("<br><b><font color=\"red\">Invalid Username/Password</font></b>");
			}
			?>
			<table>
			<tr>
				<td><img src="imgs/druidialogo.png"/></td>
				<td><label for='username'>Username:</label></td>
				<td><input type="text" name="userName" maxlength='64' value="<?= $userName ?>"/></td>
			
				<td><label for='passwd'>Password:</label></td>
				<td><input type="password" name="passWord" maxlength='64' value="<?= $pw ?>"/></td>
				<td>
				 <label for='checkbox'>Remember Me</label>
				 <input type="checkbox" name="rememberLoginCheckBox"/>
				</td>
				<td>
				 <input type="submit" name="loginSubmit" value="Log-In"/>								
				 <a href="http://www.druidia.com/register.php">Register</a>							
				</td>
			</tr>						
			</table>
			
			</form>
		</div>												
		<script type="text/javascript" language="JavaScript">
			document.forms['loginForm'].elements['userName'].focus();
		</script><?php
	}
?>