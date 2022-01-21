# JENKINS STUFF

Note that we have hardcoded paths in the docker-compose file here. This is okay for now, but for future semesters as a part of a project (robustifying CICD) we could add the ability for sed replacement/ env vars of absolute paths isntead of hard-coding them.

Really, this just boots up an instance of Jenkins.

The jenkins configs used are relatiavely simple,

- Turn on sign-up
- Create admin acct (NOT THE JENKINS MASTER ACCT, this is typically seen as bad practice [depending on how you set up Jenkins])
- Restrict all user access to read only other than admin acct
- Grant permissions as necessary

## Suggested Plugins:
# Docker (Includes many other plugins, but is a fantastic global package)
# Discord Connect/ Slack Messager (For build updates)

# How to access Jenkins:
- Be connected to onyx with x-forwarding (or on a school node)
- visit http://ubuntu-broncodrome:8080
- sign up for an account for read-only access

# Plan for semester-to-semester transition:
- Before the semester is over, ensure that either Dr.Henderson or Ben has administrative access to Jenkins, so that future students can be added as Admin.

# Notes about ports used:
- 8080: Web interface
- 50000: Master-Agent communication (more services as well)
- 5000: The ability to connecto to a docker-cloud eventually (Unless you wanted to do this via Docker Networks, which may be safer/better/more robust)

## Future Project Idea:
- Solidify Jenkins security.
- Monitor Logs remotely from non-onyx location
- User-specific Jenkins updates (email/slack/discord)


