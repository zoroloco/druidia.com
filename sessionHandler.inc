<?php
include_once 'user.inc';
include_once 'userFactory.inc';
/*
    Use the static method getInstance to get the object.
*/

class SessionHandler
{
    const SESSION_STARTED = TRUE;
    const SESSION_NOT_STARTED = FALSE;
   
    // The state of the session
    private static $sessionState = self::SESSION_NOT_STARTED;
   
    // THE only instance of the class
    private static $instance;
   
   
    private function __construct() {}
   
   public static function getSessionID(){
		return self::$sessionState;
   }
   
    /**
    *    Returns THE instance of 'Session'.
    *    The session is automatically initialized if it wasn't.
    *   
    *    @return    object
    **/
   
    public static function getInstance()
    {
        if ( !isset(self::$instance))
        {
            self::$instance = new self;
        }
       
        self::$instance->startSession();
       
        return self::$instance;
    }
   
   
    /**
    *    (Re)starts the session.
    *   
    *    @return    bool    TRUE if the session has been initialized, else FALSE.
    **/
   
    public function startSession()
    {
        if ( $this->sessionState == self::SESSION_NOT_STARTED )
        {
            $this->sessionState = session_start();
        }
       
        return $this->sessionState;
    }
   
   
    /**
    *    Stores datas in the session.
    *    Example: $instance->foo = 'bar';
    *   
    *    @param    name    Name of the datas.
    *    @param    value    Your datas.
    *    @return    void
    **/
   
    public function __set( $name , $value )
    {
		//set objects
		if($value instanceof User){			
			$_SESSION['user_Name']     = $value->userName;
			$_SESSION['user_Password'] = $value->passWord;
			$_SESSION['user_Priv']     = $value->userPriv;
			//echo("exploded user object to session");
		}
		else{//just setting simple string
			$_SESSION[$name] = $value;
		}        
    }
   
   
    /**
    *    Gets datas from the session.
    *    Example: echo $instance->foo;
    *   
    *    @param    name    Name of the datas to get.
    *    @return    mixed    Datas stored in session.
    **/
   
    public function __get( $name )	    
	{
		if( $name == 'user'){//handle user object
			//echo("its a user object");
			return UserFactory::createFromSession();
		}	
        else if ( isset($_SESSION[$name]))//handle simple string
        {
			//echo("its a string");
            return $_SESSION[$name];
        }
    }
   
   
    public function __isset( $name )
    {
        return isset($_SESSION[$name]);
    }
   
   
    public function __unset( $name )
    {
        unset( $_SESSION[$name] );
    }
   
   
    /**
    *    Destroys the current session.
    *   
    *    @return    bool    TRUE is session has been deleted, else FALSE.
    **/
   
    public function destroy()
    {
        if ( $this->sessionState == self::SESSION_STARTED )
        {
            $this->sessionState = !session_destroy();
            unset( $_SESSION );
           
            return !$this->sessionState;
        }
       
        return FALSE;
    }
}

?>