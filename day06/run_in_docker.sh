docker run  --mount type=bind,source="$(pwd)",target=/src --workdir="/src" quazeeee/gcc13-master:07-12.22 bash build.sh

