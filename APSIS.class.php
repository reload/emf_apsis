<?php

  // Apsis Newsletter Pro v2 - PHP5 example code

  // Include NUSOAP classes
  @require_once(libraries_get_path('nusoap') .'/nusoap.php');
  
  // Exceptions
  class AnpSoapClientException extends Exception
  {
    public function __construct($message = null, $code = 0)
    {
      parent::__construct($message, $code);
    }
  }
  
  // NUSOAP wrapper for ANP API
  class AnpSoapclient
  {
    protected $service_username = null, $service_password = null;
    protected $soap_client = null;
  
    public function __construct($username, $password, $wsdl = 'http://api.anpdm.com/ExternalAPIService.asmx?wsdl')
    {
      // Init soapclient
      $this->soap_client = $soap_client = new nusoap_client($wsdl, 'wsdl');
      if(($soap_error = $soap_client->getError())) $this->ThrowError($soap_error, -1, true);
      
      // Set authentication
      $this->SetAuthenticationDetails($username, $password);
    }
    
    // public array Call( string method, array arguments )
    // @method - the method to call
    // @arguments - assoc array of arguments ex; ('Parameter1' => 'Value1', 'Parameter2' => 'Value2') ..
    public function Call($method, $arguments = array())
    {
      // Init
      $soap_client = $this->soap_client;
    
      // Append anp username and password to parameters vector
      $arguments = array_merge(array('strUsername' => $this->service_username, 'strPassword' => $this->service_password), (array) $arguments);
      $soap_result = @$soap_client->Call($method, array('parameters' => $arguments), '', '', false, true);
      
      // Print error/return result
      return ($soap_client->fault ? $this->ThrowError($soap_result, -2, true) :
        (($soap_error = $soap_client->getError()) ? $this->ThrowError($soap_error, -3, true) : $soap_result));
    }
    
    protected function ThrowError($description, $error_code = -1, $dump_description = false)
    {
      if($dump_description)
      {
        @ob_start(); print_r($description);
        $description = @str_replace(chr(32) . chr(32), "&nbsp;", @nl2br(@ob_get_clean()));
      }
      throw new AnpSoapClientException($description, $error_code);
    }
    
    protected function SetAuthenticationDetails($username, $password)
    {
      if((strlen($username) == 0) || (strlen($password) == 0) || $username == null || $password == null)
        $this->ThrowError("Username or Password empty!", -5);
      
      $this->service_username = $username;
      $this->service_password = $password;
    }
  }
  
  // Demographics class
  class AnpDemographicsData extends BaseDictionary
  {
    public function __construct($demographics_data = null)
    {
      // Call base constructor
      parent::__construct();
      
      // Populate demographics data
      if($demographics_data != null && count($demographics_data) > 0)
        foreach($demographics_data as $key => $val) $this->Add($key, $val);
    }
    
    public function GetKeys($delim = ';')
    {
      return implode($delim, array_keys($this->dictionary));
    }
    
    public function GetValues($delim = ';')
    {
      return implode($delim, array_values($this->dictionary));
    }
    
    public function Remove($key)
    {
      parent::Remove("DD" . $key);
    }
    
    public function Add($key, $value)
    {
      parent::Add("DD" . $key, $value);
    }
  }
  
  class BaseDictionary
  {
    protected $dictionary = null;
    
    public function __construct()
    {
      $this->dictionary = array();
    }
    
    public function Add($key, $value)
    {
      if($this->ContainsKey($key)) throw new Exception("Dictionary already contains key {$key}");
      $this->dictionary[$key] = $value;
    }
    
    public function Remove($key)
    {
      unset($this->dictionary[$key]);
    }
    
    public function Get($key)
    {
      return $this->dictionary[$key];
    }
    
    public function ContainsKey($key)
    {
      return array_key_exists($key, $this->dictionary);
    }
  }

?>
