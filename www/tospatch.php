<?php

error_reporting(E_ALL & ~E_WARNING);
error_reporting(E_ALL);
ini_set("display_errors", 1);
ini_set("track_errors", 1);

$top = getcwd();
$custom_dir = 'custom/';

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
flock($log, LOCK_EX);
$currdate = date('Y-m-d H:i:s');
fputs($log, "$currdate: start\n");
fprintf($log, "FROM: %s\n", $_SERVER['REMOTE_ADDR']);
fprintf($log, "POST: %s\n", var_export($_POST, true));
fprintf($log, "FILES: %s\n", var_export($_FILES, true));
fprintf($log, "SERVER: %s\n", var_export($_SERVER, true));

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
	global $ramversion;

	$retval = false;
	$ramversion = 0;
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
			case "--ramversion":
				$i++;
				$ramversion = $_SERVER["argv"][$i];
				break;
			}
		}
	} else
	{
		if (isset($_POST['tosversion']))
		{
			$tosversion = $_POST['tosversion'];
		}
		if (isset($_POST['country']))
		{
			$country = $_POST['country'];
		}
		$ramversion = isset($_POST['ramversion']) ? $_POST['ramversion'] : 0;
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
	global $ramversion;
	global $filename;
	global $zip;
	global $exitcode;
	global $custom_dir;
	
	$retval = false;
	$exitcode = 1;
	
	$fp = fopen('localcnf.mak', 'w');
	if (!is_resource($fp))
	{
		error_log("$php_errormsg");
	} else
	{
		fprintf($fp, "BINEXACT:=0\n");
		fprintf($fp, "TOSVERSION:=%d\n", $tosversion);
		fprintf($fp, "COUNTRY:=%s\n", $country);
		fprintf($fp, "RAMVERSION:=%s\n", $ramversion);
		fclose($fp);
	
		$fp = fopen('common/localcnf.h', 'w');
		$mk = fopen('glue/localrsc.mak', 'w');
		if (!is_resource($fp) || !is_resource($mk))
		{
			error_log("$php_errormsg");
		} else
		{
			$tp_01 = isset($_POST['tp_01']) ? $_POST['tp_01'] : 0;
			$tp_02 = isset($_POST['tp_02']) ? $_POST['tp_02'] : 0;
			$tp_03 = isset($_POST['tp_03']) ? $_POST['tp_03'] : 0;
			$tp_04 = isset($_POST['tp_04']) ? $_POST['tp_04'] : 0;
			$tp_05 = isset($_POST['tp_05']) ? $_POST['tp_05'] : 0;
			$tp_06 = isset($_POST['tp_06']) ? $_POST['tp_06'] : 0;
			$tp_07 = isset($_POST['tp_07']) ? $_POST['tp_07'] : 0;
			$tp_08 = isset($_POST['tp_08']) ? $_POST['tp_08'] : 0;
			$tp_09 = isset($_POST['tp_09']) ? $_POST['tp_09'] : 0;
			$tp_10 = isset($_POST['tp_10']) ? $_POST['tp_10'] : 0;
			$tp_11 = isset($_POST['tp_11']) ? $_POST['tp_11'] : 0;
			$tp_12 = isset($_POST['tp_12']) ? $_POST['tp_12'] : 0;
			$tp_13 = isset($_POST['tp_13']) ? $_POST['tp_13'] : 0;
			$tp_14 = isset($_POST['tp_14']) ? $_POST['tp_14'] : 0;
			$tp_15 = isset($_POST['tp_15']) ? $_POST['tp_15'] : 0;
			$tp_16 = isset($_POST['tp_16']) ? $_POST['tp_16'] : 0;
			$tp_17 = isset($_POST['tp_17']) ? $_POST['tp_17'] : 0;
			$tp_18 = isset($_POST['tp_18']) ? $_POST['tp_18'] : 0;
			$tp_19 = isset($_POST['tp_19']) ? $_POST['tp_19'] : 0;
			$tp_20 = isset($_POST['tp_20']) ? $_POST['tp_20'] : 0;
			$tp_21 = isset($_POST['tp_21']) ? $_POST['tp_21'] : 0;
			$tp_22 = isset($_POST['tp_22']) ? $_POST['tp_22'] : 0;
			$tp_23 = isset($_POST['tp_23']) ? $_POST['tp_23'] : 0;
			$tp_24 = isset($_POST['tp_24']) ? $_POST['tp_24'] : 0;
			$tp_25 = isset($_POST['tp_25']) ? $_POST['tp_25'] : 0;
			$tp_26 = isset($_POST['tp_26']) ? $_POST['tp_26'] : 0;
			$tp_27 = isset($_POST['tp_27']) ? $_POST['tp_27'] : 0;
			$tp_28 = isset($_POST['tp_28']) ? $_POST['tp_28'] : 0;
			$tp_29 = isset($_POST['tp_29']) ? $_POST['tp_29'] : 0;
			$tp_30 = isset($_POST['tp_30']) ? $_POST['tp_30'] : 0;
			$tp_31 = isset($_POST['tp_31']) ? $_POST['tp_31'] : 0;
			$tp_32 = isset($_POST['tp_32']) ? $_POST['tp_32'] : 0;
			$tp_33 = isset($_POST['tp_33']) ? $_POST['tp_33'] : 0;
			$tp_34 = isset($_POST['tp_34']) ? $_POST['tp_34'] : 0;
			$tp_35_6 = isset($_POST['tp_35_6']) ? $_POST['tp_35_6'] : 0;
			$tp_35_8 = isset($_POST['tp_35_8']) ? $_POST['tp_35_8'] : 0;
			$tp_35_16 = isset($_POST['tp_35_16']) ? $_POST['tp_35_16'] : 0;
			$tp_35_32 = isset($_POST['tp_35_32']) ? $_POST['tp_35_32'] : 0;
			$tp_36 = isset($_POST['tp_36']) ? $_POST['tp_36'] : '';

			$tp_40 = isset($_POST['tp_40']) ? $_POST['tp_40'] : 0;
			$tp_41 = isset($_POST['tp_41']) ? $_POST['tp_41'] : 0;
			$tp_42 = isset($_POST['tp_42']) ? $_POST['tp_42'] : 0;
			$tp_43 = isset($_POST['tp_43']) ? $_POST['tp_43'] : 0;
			$tp_44 = isset($_POST['tp_44']) ? $_POST['tp_44'] : 0;
			$tp_45 = isset($_POST['tp_45']) ? $_POST['tp_45'] : 0;
			$tp_46 = isset($_POST['tp_46']) ? $_POST['tp_46'] : 0;
			$tp_47 = isset($_POST['tp_47']) ? $_POST['tp_47'] : 0;
			$tp_48 = isset($_POST['tp_48']) ? $_POST['tp_48'] : 0;
			$tp_49 = isset($_POST['tp_49']) ? $_POST['tp_49'] : 0;
		
			$seekrate = isset($_POST['seekrate']) ? $_POST['seekrate'] : 3;
			$fdc_cookie = isset($_POST['fdc_cookie']) ? $_POST['fdc_cookie'] : '$01415443';
			$boottime = isset($_POST['boottime']) ? $_POST['boottime'] : 80;
			$prntimeout = isset($_POST['prntimeout']) ? $_POST['prntimeout'] : 30;
			$conterm = isset($_POST['conterm']) ? $_POST['conterm'] : 7;

			fprintf($fp, "#define TP_01 %d\n", $tp_01);
			fprintf($fp, "#define TP_02 %d\n", $tp_02);
			fprintf($fp, "#define TP_03 %d\n", $tp_03);
			fprintf($fp, "#define TP_04 %d\n", $tp_04);
			fprintf($fp, "#define TP_05 %d\n", $tp_05);
			fprintf($fp, "#define TP_06 %d\n", $tp_06);
			fprintf($fp, "#define TP_07 %d\n", $tp_07);
			fprintf($fp, "#define TP_08 %d\n", $tp_08);
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
			fprintf($fp, "#define TP_30 %d\n", $tp_30);
			fprintf($fp, "#define TP_31 %d\n", $tp_31);
			fprintf($fp, "#define TP_32 %d\n", $tp_32);
			fprintf($fp, "#define TP_33 %d\n", $tp_33);
			fprintf($fp, "#define TP_34 %d\n", $tp_34);
			fprintf($fp, "#define TP_35_6 %d\n", $tp_35_6);
			fprintf($fp, "#define TP_35_8 %d\n", $tp_35_8);
			fprintf($fp, "#define TP_35_16 %d\n", $tp_35_16);
			fprintf($fp, "#define TP_35_32 %d\n", $tp_35_32);
			fprintf($fp, "#define TP_36 \"%s\"\n", $tp_36);

			for ($icon = 1; $icon <= 12; $icon++)
			{
				if (isset($_FILES['tp_37_' . $icon]['tmp_name']) &&
					$_FILES['tp_37_' . $icon]['error'] == UPLOAD_ERR_OK)
				{
					$icon_name = $custom_dir . 'tp_37_' .  $icon . '.ico';
					move_uploaded_file($_FILES['tp_37_' . $icon]['tmp_name'], $icon_name);
					fprintf($fp, "#define TP_37_%d \"../%s\"\n", $icon, $icon_name);
				}
			}

			for ($icon = 0; $icon <= 7; $icon++)
			{
				if (isset($_FILES['tp_38_' . $icon]['tmp_name']) &&
					$_FILES['tp_38_' . $icon]['error'] == UPLOAD_ERR_OK)
				{
					$icon_name = $custom_dir . 'tp_38_' .  $icon . '.ico';
					move_uploaded_file($_FILES['tp_38_' . $icon]['tmp_name'], $icon_name);
					fprintf($fp, "#define TP_38_%d \"../%s\"\n", $icon, $icon_name);
				}
			}

			for ($icon = 1; $icon <= 3; $icon++)
			{
				if (isset($_FILES['tp_39_' . $icon]['tmp_name']) &&
					$_FILES['tp_39_' . $icon]['error'] == UPLOAD_ERR_OK)
				{
					$icon_name = $custom_dir . 'tp_39_' .  $icon . '.ico';
					move_uploaded_file($_FILES['tp_39_' . $icon]['tmp_name'], $icon_name);
					fprintf($fp, "#define TP_39_%d \"../%s\"\n", $icon, $icon_name);
				}
			}

			$rsc_names = array(
				'',
				'GEM_RSC',
				'DESK_RSC',
				'DESK_INF',
			);
			for ($icon = 1; $icon <= 3; $icon++)
			{
				if (isset($_FILES['tp_40_' . $icon]['tmp_name']) &&
					$_FILES['tp_40_' . $icon]['error'] == UPLOAD_ERR_OK)
				{
					$icon_name = $custom_dir . 'tp_40_' .  $icon . '.ico';
					move_uploaded_file($_FILES['tp_40_' . $icon]['tmp_name'], $icon_name);
					fprintf($mk, "%s := ../%s\n", $rsc_names[$icon], $icon_name);
				}
			}

			fprintf($fp, "#define TP_40 %d\n", $tp_40);
			fprintf($fp, "#define TP_41 %d\n", $tp_41);
			fprintf($fp, "#define TP_42 %d\n", $tp_42);
			fprintf($fp, "#define HD_WAIT %d\n", $tp_43);
			fprintf($fp, "#define HD_WAITTIME %d\n", $tp_44);
			fprintf($fp, "#define HD_WAITDEVICE %d\n", $tp_45);
			fprintf($fp, "#define TP_46 %d\n", $tp_46);
			fprintf($fp, "#define TP_47 %d\n", $tp_47);
			fprintf($fp, "#define TP_48 %d\n", $tp_48);
			fprintf($fp, "#define TP_49 %d\n", $tp_49);

			fprintf($fp, "#define STEP_RATE %d\n", $seekrate);
			fprintf($fp, "#define FDC_COOKIE %s\n", $fdc_cookie);
			fprintf($fp, "#define BOOT_TIME %d\n", $boottime);
			fprintf($fp, "#define PRNTIMEOUT %d\n", $prntimeout);
			fprintf($fp, "#define CONTERM %d\n", $conterm);

			fclose($fp);
			fclose($mk);
			
			system("make clean 2>&1");
			system("make 2>&1", $exitcode);
			
			if ($exitcode != 0)
			{
				error_log("make exited with code $exitcode");
			} else
			{
				system("zip -j $zip $filename 2>&1", $exitcode);
				if ($exitcode != 0)
				{
					error_log("zip exited with code $exitcode");
				} else
				{
					$retval = true;
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

if ($retval)
	$retval = compile_tos();

$compile_output = ob_get_contents();
ob_end_clean();

/* fputs($log, "$compile_output\n"); */

if ($retval)
{
	if (headers_sent($file, $line))
	{
		error_log("headers already sent at $file $line");
	} else
	{
		send_headers();
		header('Content-Length: ' . filesize($zip));
	}
	flush();
	readfile($zip);
} else
{
	html_header();
	/* echo $compile_output; */
	echo "error generating tos.img\n";

	$errfile = fopen('../errors.log', 'a');
	fputs($errfile, "$currdate: start\n");
	fprintf($errfile, "FROM: %s\n", $_SERVER['REMOTE_ADDR']);
	fprintf($errfile, "POST: %s\n", var_export($_POST, true));
	fprintf($errfile, "FILES: %s\n", var_export($_FILES, true));
	fprintf($errfile, "FAILED:\n");
	fputs($errfile, "$compile_output\n\n");
	fclose($errfile);

	html_trailer();
}

$currdate = date('Y-m-d H:i:s');
fprintf($log, "$currdate: end (%s)\n\n", $retval ? "success" : "failed");


fclose($log);
	
?>
