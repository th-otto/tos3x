<?php

error_reporting(E_ALL & ~E_WARNING);
error_reporting(E_ALL);
ini_set("display_errors", 1);
ini_set("track_errors", 1);

$top = getcwd();

function html_header()
{
	header_remove();

	echo "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n";
	echo "          \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n";
	echo "<html xml:lang=\"en\" lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">\n";
	echo "<head>\n";
	echo "<meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\" />\n";
	echo "<pre>\n";
}


function send_headers()
{
	global $zip;
	
	header('Content-Description: File Transfer');
	header('Content-Type: application/x-zip-compressed');
	header('Content-Transfer-Encoding: binary');
	header('Expires: 0');
	header('Cache-Control: must-revalidate');
	header('Pragma: public');
	header('Content-Disposition: attachment; filename="'.basename($zip).'"');
}


function html_trailer()
{
	echo "</pre>\n";
	echo "</body>\n";
	echo "</html>\n";
}


function is_cli()
{
	return substr(php_sapi_name(), 0, 3) == 'cli';
}
	

$log = fopen('tospatch.log', 'a');
$currdate = date('Y-m-d H:i:s');
fputs($log, "$currdate: start\n");
fprintf($log, "FROM: %s\n", $_SERVER['REMOTE_ADDR']);
fprintf($log, "QUERY_STRING: %s\n", $_SERVER['QUERY_STRING']);

function error_handler($errno, $errmsg, $errfile, $errline, $errcontext)
{
	global $log;
	fputs($log, "$errmsg\n");
	return true;
}


function log_output($str)
{
	return '';
}

set_error_handler('error_handler', E_ALL);


function get_options()
{
	global $tosversion;
	global $country;

	$retval = false;
	if (is_cli())
	{
		for ($i = 1; $i < $_SERVER["argc"]; $i++)
		{
			switch($_SERVER["argv"][$i])
			{
			case "--tosversion":
				$i++;
				$tosversion = $_SERVER["argv"][$i];
				break;
			case "--country":
				$i++;
				$country = $_SERVER["argv"][$i];
				break;
			}
		}
	} else
	{
		if (isset($_GET['tosversion']))
		{
			$tosversion = $_GET['tosversion'];
		}
		if (isset($_GET['country']))
		{
			$country = $_GET['country'];
		}
	}
	if ($tosversion == 0)
	{
		echo "TOSVERSION is not defined\n";
	} else if ($country == '')
	{
		echo "COUNTRY is not defined\n";
	} else
	{
		$retval = true;
	}
	return $retval;
}


function compile_tos()
{
	global $tosversion;
	global $country;
	global $filename;
	global $zip;
	
	$retval = false;
	$exitcode = 0;
	
	if (is_cli() || $_SERVER['REQUEST_METHOD'] == 'GET')
	{
		$fp = fopen('localcnf.mak', 'w');
		if (!is_resource($fp))
		{
			error_log("$php_errormsg");
		} else
		{
			fprintf($fp, "BINEXACT=0\n");
			fprintf($fp, "TOSVERSION=%d\n", $tosversion);
			fprintf($fp, "COUNTRY=%s\n", $country);
			fprintf($fp, "LOCALCONF='\\#include \"localcnf.h\"'\n");
			fclose($fp);
		
			$tp_01 = isset($_GET['tp_01']) ? $_GET['tp_01'] : 0;
			$tp_02 = isset($_GET['tp_02']) ? $_GET['tp_02'] : 0;
			$tp_03 = isset($_GET['tp_03']) ? $_GET['tp_03'] : 0;
			$tp_04 = isset($_GET['tp_04']) ? $_GET['tp_04'] : 0;
			$tp_05 = isset($_GET['tp_05']) ? $_GET['tp_05'] : 0;
			$tp_06 = isset($_GET['tp_06']) ? $_GET['tp_06'] : 0;
			$tp_07 = isset($_GET['tp_07']) ? $_GET['tp_07'] : 0;
			$seekrate = isset($_GET['seekrate']) ? $_GET['seekrate'] : 3;
			$tp_08 = isset($_GET['tp_08']) ? $_GET['tp_08'] : 0;
			$fdc_cookie = isset($_GET['fdc_cookie']) ? $_GET['fdc_cookie'] : '$01415443';
			$tp_09 = isset($_GET['tp_09']) ? $_GET['tp_09'] : 0;
			$tp_10 = isset($_GET['tp_10']) ? $_GET['tp_10'] : 0;
			$tp_11 = isset($_GET['tp_11']) ? $_GET['tp_11'] : 0;
			$tp_12 = isset($_GET['tp_12']) ? $_GET['tp_12'] : 0;
			$tp_13 = isset($_GET['tp_13']) ? $_GET['tp_13'] : 0;
			$tp_14 = isset($_GET['tp_14']) ? $_GET['tp_14'] : 0;
			$tp_15 = isset($_GET['tp_15']) ? $_GET['tp_15'] : 0;
			$tp_16 = isset($_GET['tp_16']) ? $_GET['tp_16'] : 0;
			$tp_17 = isset($_GET['tp_17']) ? $_GET['tp_17'] : 0;
			$tp_18 = isset($_GET['tp_18']) ? $_GET['tp_18'] : 0;
			$tp_19 = isset($_GET['tp_19']) ? $_GET['tp_19'] : 0;
			$tp_20 = isset($_GET['tp_20']) ? $_GET['tp_20'] : 0;
			$tp_21 = isset($_GET['tp_21']) ? $_GET['tp_21'] : 0;
			$tp_22 = isset($_GET['tp_22']) ? $_GET['tp_22'] : 0;
			$tp_23 = isset($_GET['tp_23']) ? $_GET['tp_23'] : 0;
			$tp_24 = isset($_GET['tp_24']) ? $_GET['tp_24'] : 0;
			$tp_25 = isset($_GET['tp_25']) ? $_GET['tp_25'] : 0;
			$tp_26 = isset($_GET['tp_26']) ? $_GET['tp_26'] : 0;
			$tp_27 = isset($_GET['tp_27']) ? $_GET['tp_27'] : 0;
			$tp_28 = isset($_GET['tp_28']) ? $_GET['tp_28'] : 0;
			$tp_29 = isset($_GET['tp_29']) ? $_GET['tp_29'] : 0;
		
			$fp = fopen('common/localcnf.h', 'w');
			if (!is_resource($fp))
			{
				error_log("$php_errormsg");
			} else
			{
				fprintf($fp, "#define TP_01 %d\n", $tp_01);
				fprintf($fp, "#define TP_02 %d\n", $tp_02);
				fprintf($fp, "#define TP_03 %d\n", $tp_03);
				fprintf($fp, "#define TP_04 %d\n", $tp_04);
				fprintf($fp, "#define TP_05 %d\n", $tp_05);
				fprintf($fp, "#define TP_06 %d\n", $tp_06);
				fprintf($fp, "#define TP_07 %d\n", $tp_07);
				fprintf($fp, "#define STEP_RATE %d\n", $seekrate);
				fprintf($fp, "#define TP_08 %d\n", $tp_08);
				fprintf($fp, "#define FDC_COOKIE %s\n", $fdc_cookie);
				fprintf($fp, "#define TP_09 %d\n", $tp_09);
				fprintf($fp, "#define TP_10 %d\n", $tp_10);
				fprintf($fp, "#define TP_11 %d\n", $tp_11);
				fprintf($fp, "#define TP_12 %d\n", $tp_12);
				fprintf($fp, "#define TP_13 %d\n", $tp_13);
				fprintf($fp, "#define TP_14 %d\n", $tp_14);
				fprintf($fp, "#define TP_15 %d\n", $tp_15);
				fprintf($fp, "#define TP_16 %d\n", $tp_16);
				fprintf($fp, "#define TP_17 %d\n", $tp_17);
				fprintf($fp, "#define TP_18 %d\n", $tp_18);
				fprintf($fp, "#define TP_19 %d\n", $tp_19);
				fprintf($fp, "#define TP_20 %d\n", $tp_20);
				fprintf($fp, "#define TP_21 %d\n", $tp_21);
				fprintf($fp, "#define TP_22 %d\n", $tp_22);
				fprintf($fp, "#define TP_23 %d\n", $tp_23);
				fprintf($fp, "#define TP_24 %d\n", $tp_24);
				fprintf($fp, "#define TP_25 %d\n", $tp_25);
				fprintf($fp, "#define TP_26 %d\n", $tp_26);
				fprintf($fp, "#define TP_27 %d\n", $tp_27);
				fprintf($fp, "#define TP_28 %d\n", $tp_28);
				fprintf($fp, "#define TP_29 %d\n", $tp_29);
				fclose($fp);
			
				system("make clean 2>&1");
				system("make 2>&1", $exitcode);
				
				if ($exitcode == 0)
				{
					system("zip -j $zip $filename 2>&1", $exitcode);
					if ($exitcode == 0)
					{
						$retval = true;
					}
				}
			}
		}
	}

	return $retval;
}

chdir('src');
	
ob_clean();
ob_start("log_output");

$retval = get_options();

$filename = 'glue/tos' . $tosversion . $country . '.img';
$zip = 'glue/tos' . $tosversion . $country . '.zip';
unlink($zip);

send_headers();

if ($retval)
	$retval = compile_tos();

$compile_output = ob_get_contents();
ob_end_clean();

fputs($log, "$compile_output\n");

if ($retval)
{
	if (headers_sent($file, $line))
	{
		error_log("headers already sent at $file $line");
	}
	header('Content-Length: ' . filesize($zip));
	flush();
	readfile($zip);
} else
{
	html_header();
	echo $compile_output;
	echo "error generating tos.img\n";
	html_trailer();
}

$currdate = date('Y-m-d H:i:s');
fputs($log, "$currdate: end\n\n");


fclose($log);
	
?>
