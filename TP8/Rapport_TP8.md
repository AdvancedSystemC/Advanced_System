# TP 8 : "Handle" Docker :

#### By DUVERNAY Benoit & Gerberon Alexandre

### I - Proposer un service


Pré-requis: Installation de docker

```batch
docker pull jenkins:2.60.3
docker run -p 8080:8080 --name jenkins-container -d jenkins:2.60.3
```



![jenkins.png](C:\Users\duver\Pictures\Screenshots\jenkins.png)



```bash
docker stop jenkins-container
```



### II - Service "from scratch"

```batch
docker pull tomcat:9
```

- Téléchargment de l'archive : https://jenkins.io/

- Création d'un dockerfile : 
  
  ```dockerfile
  FROM tomcat:9
  MAINTAINER DUVERNAY
  
  COPY ./ /usr/local/tomcat/webapps/
  
  EXPOSE 8080
  ```
  
  ```batch
  docker build -t jenkins-tomcat .
  docker run -p 8080:8080 --name jenkins-tomcat-container -d jenkins-tomcat
  docker stop jenkins-tomcat-container
  ```
  
  ![jenkins2.png](C:\Users\duver\Pictures\Screenshots\jenkins2.png)


