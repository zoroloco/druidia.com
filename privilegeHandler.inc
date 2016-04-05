<?php

include_once 'databaseHandler.inc';

class PrivilegeHandler{

	const PRIV_ADMIN       = "ADMIN";
	const PRIV_GLOBAL_VIEW = "GLOBAL_VIEW";
	const PRIV_GUEST       = "GUEST";
	const PRIV_AUDI        = "AUDI";

	private $userPriv = null;
	private $privMap  = array();

	function PrivilegeHandler($user){		
		if(null != $user && isSet($user)){
			$this->userPriv = $user->userPriv;
		
			if(null != $this->userPriv &&
			   isset($this->userPriv) &&
			   $this->userPriv>0){
			   
			   $query    = "select * from privileges";
			   $result   = DataBaseHandler::query($query);		   		  		   
			   $num_rows = mysql_num_rows($result);
		
			   //build up priv map
			   if($num_rows){							
					while($row = mysql_fetch_array($result)){		
						$this->privMap[$row['NAME']] = $row['VALUE'];					
					}
			   }
			}				
		}			
	}
		
	function hasPrivilege($privName){
		$privMapKeys = array_keys($this->privMap);
		$privValue   = null;
		foreach($privMapKeys as $curPrivMapKey){
			if(0 == strcmp($curPrivMapKey,$privName)){
				$privValue = $this->privMap[$curPrivMapKey];	

				if($privValue == (intval($this->userPriv) & intval($privValue)))
					return true;
				else
					break;
			}
		}										
		return false;
	}			
}
?>