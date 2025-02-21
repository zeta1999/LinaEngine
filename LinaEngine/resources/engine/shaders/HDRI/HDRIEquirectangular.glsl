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
layout (location = 0) in vec3 position;
out vec3 LocalPos;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    LocalPos = position;
    gl_Position =  projection * view * vec4(LocalPos, 1.0);
}

#elif defined(FS_BUILD)
#include <../MaterialSamplers.glh>
out vec4 fragColor;
in vec3 LocalPos;

struct Material
{
  MaterialSampler2D equirectangularMap;
};
uniform Material material;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(LocalPos)); // make sure to normalize localPos
    vec3 color = material.equirectangularMap.isActive ? texture(material.equirectangularMap.texture, uv).rgb : vec3(0.0);
    fragColor = vec4(color, 1.0);
}
#endif
