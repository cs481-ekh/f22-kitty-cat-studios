# Before running this script, set the UNREAL_HOME environment variable
# (i.e. "SET UNREAL_HOME <path_to_unreal>")

$PROJECT_HOME=(Get-Location).Path

Write-Output "REMOVING OLD BRONCODROME ARTIFACTS"
Remove-Item -Recurse Output\*
Write-Output "ARTIFACTS REMOVED"

Write-Output "COMPILING"
Set-Location "D:\Program Files\Epic Games\UE_4.26Chaos\Engine\Build\BatchFiles"
Start-Process -NoNewWindow -FilePath "D:\Program Files\Epic Games\UE_4.26Chaos\Engine\Build\BatchFiles\RunUAT" -ArgumentList "BuildCookRun -project=$PROJECT_HOME\BroncoDrome.uproject -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory=$PROJECT_HOME\Output"

if($COOK_PROJECT -eq $True)
{
    Write-Output "COOKING"
    Start-Process -NoNewWindow -FilePath "D:\Program Files\Epic Games\UE_4.26Chaos\Engine\Build\BatchFiles\RunUAT" -ArgumentList "BuildCookRun -project=$PROJECT_HOME\BroncoDrome.uproject -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -NoCompile -stage -pak -archive -archivedirectory=$PROJECT_HOME\Output"
}

Set-Location $PROJECT_HOME