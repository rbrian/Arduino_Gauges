void localLogger(Logger::Level level, const char* module, const char* message)
{
  time_t t=now();
  Serial.printf("[%02i:%02i:%02i - %s]",hour(t),minute(t),second(t),Logger::asString(level));
  if (strlen(module) > 0)
  {
      Serial.printf(":%s", module);
  }
  Serial.println(message);
}
