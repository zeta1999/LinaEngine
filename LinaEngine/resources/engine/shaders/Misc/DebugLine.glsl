/*
 * Copyright (C) 2019 Inan Evin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#if defined(VS_BUILD)
#include <../UniformBuffers.glh>
layout (location = 0) in vec3 position;

void main()
{
  gl_Position = projection * view * vec4(position, 1.0);
}

#elif defined(FS_BUILD)
out vec4 fragColor;
struct Material
{
  vec3 color;
};
uniform Material material;

void main()
{
   fragColor = vec4(material.color.x, material.color.y, material.color.z, 1);
}
#endif
