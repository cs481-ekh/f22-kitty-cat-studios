pipeline {
    agent {label 'windowsomen'}
    stages {
        stage('Pull changes') {
            steps {
                git branch: 'deployment', credentialsId: 'c9d65806-b996-4fa9-89c0-d45fa0ed79f6', url: 'https://github.com/cs481-ekh/f22-kitty-cat-studios'
            }
        }
    
        stage('Build and Test') {
            steps {
                withEnv(['UNREAL_HOME="D:\\Program Files\\Epic Games\\UE_4.26Chaos"']) {
                    powershell 'D:\\workspace\\BroncoDrome\\build.ps1'
                }
            }
        }
        
        stage('Archive package') {
            steps {
                archiveArtifacts artifacts: 'Output\\WindowsNoEditor', followSymlinks: false
            }
        }
    }
}
