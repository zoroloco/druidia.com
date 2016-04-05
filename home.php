<?php
include_once 'sessionHandler.inc';
include_once 'htmlUtil.inc';
include_once 'board.inc';
include_once 'notes.inc';
include_once 'userEditor.inc';

run();

function run(){
	$userSession = 'user';
	$user        = SessionHandler::getInstance()->$userSession;		

	if($_SERVER['REQUEST_METHOD']=='GET'){
		processGetRequest($user);
	}else if($_SERVER['REQUEST_METHOD']=='POST'){
		//var_dump($user);
		processPostRequest($user);
	}else{
		die("This script only works with GET and POST requests.");
	}		
}

function processGetRequest($user){	
	$user = validateLogin($user);//note: reroute back to index.php if fail
	
	if('true'==$_GET['cookieFlag']){		
		setcookie("cookieUserName", session_id(), time()+3600*24, "/", "www.druidia.com",FALSE);
	}

    HtmlUtil::sendHeader('druidia.com',TRUE);        
	
	$editFlag      = $_GET['edit'];
	
    if(null != $editFlag && 
       isSet($editFlag) &&
       $editFlag){
    	UserEditor::processGetRequest($user);
    }	
    else{
    	displayUserHeader($user);
		Board::processGetRequest($user);
		Notes::processGetRequest($user);
		displayHomeRight($user);
    }
    
    HtmlUtil::sendFooter();
}

function processPostRequest($user){		
	HtmlUtil::sendHeader('druidia.com',TRUE);
	
	if(!empty($_POST['editSubmit'])){
		UserEditor::processPostRequest($user);
	}
	else{
		displayUserHeader($user);
		Board::processPostRequest($user);
		Notes::processPostRequest($user);
		displayHomeRight($user);
	}
	
	HtmlUtil::sendFooter();
}
		
function validateLogin($user){
	if(null != $user && isset($user)){//if logged-in then proceed		
		//get the FULL user object from DB
		$user = UserFactory::createFullFromQuery($user->userName);
		return $user;
	}
	else{
		echo("Please log-in!");	//re-route back to login screen
		HtmlUtil::redirect('index.php');
		exit(0);
	}	
	
	return $user;
}		
		
function displayUserHeader($user){
	if(isset($user)){
		echo("<div id=\"UserHeader\">Welcome - ".$user->userName."<br>");
		?>
		[<a href="http://www.druidia.com/index.php?logOut=true">Log Out</a>]
		[<a href="http://www.druidia.com/home.php?edit=true">My Account</a>]
		<?php
		echo("</div>");
	}	
}

function displayHomeRight($user){
	
	
}
		
?>
	</body>		
</html>