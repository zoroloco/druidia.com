<?php
include_once 'user.inc';
include_once 'sessionHandler.inc';

class UserFactory{

	function createFromSession(){
		$user = null;
		$session = SessionHandler::getInstance();
		if(isset($session->user_Name)){
			$user = new User();
			$user->userName = $session->user_Name;
			
			if(isset($session->user_Password)){
				$user->passWord = $session->user_Password;
			}
			if(isset($session->user_Priv)){
				$user->userPriv = $session->user_Priv;
			}
		}		
		
		return $user;
	}
	
	function createFromPost($post){
		//var_dump($post);
		$user = new User();
	
		$user->userName = $post['userName'];
		$user->passWord = $post['passWord'];
		$user->lastName = $post['lastName'];
		$user->firstName= $post['firstName'];
		$user->sex      = $post['sex'];	
		$user->email    = $post['email'];
		$user->userImagePath=$post['userImagePath'];	
		//user image extracted in register.php form
		return $user;
	}
	
	function createFromQuery($result){
		$user = null;
	
		while($row = mysql_fetch_array($result))
		{	
			if(null == $user)
				$user = new User();
				
			//print_r($row);	
				
			//populate
			$user->userName = $row['USER_NAME'];
			$user->passWord = $row['PASSWORD'];
			$user->userPriv = $row['USER_PRIV'];
			$user->lastName = $row['LAST_NAME'];
			$user->firstName= $row['FIRST_NAME'];
			$user->userImagePath= $row['USER_IMAGE_PATH'];
			$user->email    = $row['EMAIL'];
			//echo("creating user with userpriv=".$user->userPriv);
		}
		
		return $user;
	}
	
	/**
	* The cookie holds the user name.  With the user name we will query
	* DB and get the rest of the user fields and build the user object.
	**/
	function createFromCookie(){
		if(isset($_COOKIE['cookieUserName'])){		
			$cookiedSessionId = $_COOKIE['cookieUserName'];
			//echo("<br>".$cookiedSessionId."</br>");
			$query = "select * from user u where u.SESSION_VAR =\"".$cookiedSessionId."\"";				
			//echo("<br>".$query."</br>");
			$result   = DataBaseHandler::query($query);		   		  		   
			$num_rows = mysql_num_rows($result);
			//echo($num_rows);
			if($num_rows){	
				return self::createFromQuery($result);
			}
		}			
		
		return null;
	}
	
	function createFullFromQuery($un){
		$query = "select * from user u where u.USER_NAME =\"".strtoupper($un)."\"";

		$result   = DataBaseHandler::query($query);		   		  		   
		$num_rows = mysql_num_rows($result);
		
		if(!$num_rows){								   											
			return null;
		}
		else{					
			return UserFactory::createFromQuery($result);
		}		   		   
	}
	
	function storeUserInPost($user){
		echo("<input type=\"hidden\" name=\"userName\" value=\"" . $user->userName . "\"/>");
		echo("<input type=\"hidden\" name=\"lastName\" value=\"" . $user->lastName . "\"/>");
		echo("<input type=\"hidden\" name=\"firstName\" value=\"" . $user->firstName . "\"/>");
		echo("<input type=\"hidden\" name=\"userImagePath\" value=\"" . $user->userImagePath . "\"/>");
	}
}

?>