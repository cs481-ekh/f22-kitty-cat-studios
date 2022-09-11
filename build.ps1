# Before running this script, set the UNREAL_HOME environment variable 
# (i.e. "SET UNREAL_HOME <path_to_unreal>")

$PROJECT_HOME=(Get-Location).Path

if($UNREAL_HOME -eq $null)
{
	Write-Output "Please set the UNREAL_HOME environment variable!"
}

Write-Output "REMOVING OLD BRONCODROME ARTIFACTS"
Remove-Item -Recurse Output\*
Write-Output "ARTIFACTS REMOVED"

Write-Output "COMPILING"
Set-Location $UNREAL_HOME\Engine\Build\BatchFiles
Start-Process -NoNewWindow -FilePath "$UNREAL_HOME\Engine\Build\BatchFiles\RunUAT" -ArgumentList "BuildCookRun -project=$PROJECT_HOME\BroncoDrome.uproject -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory=$PROJECT_HOME\Output"

if($COOK_PROJECT -eq $True)
{
    Write-Output "COOKING"
    Start-Process -NoNewWindow -FilePath "$UNREAL_HOME\Engine\Build\BatchFiles\RunUAT" -ArgumentList "BuildCookRun -project=$PROJECT_HOME\BroncoDrome.uproject -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -NoCompile -stage -pak -archive -archivedirectory=$PROJECT_HOME\Output"
}

Set-Location $PROJECT_HOME